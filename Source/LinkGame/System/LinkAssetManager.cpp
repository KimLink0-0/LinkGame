// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LinkAssetManager.h"

#include "LinkLogChannels.h"

ULinkAssetManager::ULinkAssetManager()
{
}

ULinkAssetManager& ULinkAssetManager::Get()
{
	check(GEngine);
	
	// LinkAssetManager 는 UEngine 의 GEngine 의 AssetManager 의 class 를 오버라이드 했기 때문에 AssetManager 가 존재한다
	ULinkAssetManager* Singleton = Cast<ULinkAssetManager>(GEngine->AssetManager);
	if (Singleton)
	{
		return *Singleton;
	}
	
	UE_LOG(LogLink, Fatal, TEXT("invalid AssetManagerClassname in DefaultEngine.ini(protect settings); it must be LinkAssetManager"));
	
	// 위 Fatal LOG 에 의해 도달하진 않지만, return 값이 필요하므로 Compile 을 위해 더미로 존재
	return *NewObject<ULinkAssetManager>();
}

bool ULinkAssetManager::ShouldLogAssetLoads()
{
	// 에셋 로드 순서나 병목 현상을 디버깅하고 싶을 때 로그를 사용하기 위함
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

UObject* ULinkAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// Synchronous Load Asset 이 불필요하게 하는 것이 없는지 확인하기 위함
	// 동기식 로딩에 문제가 있는지 찾기 위해서 (Frame Drop, 시간 지연  등)
	// 동기 로딩을 그대로 사용하지만, 시간 체크를 하기 위해 한 번 랩핑한 함수
	if (AssetPath.IsValid())
	{
		// FScopeLogTime 을 이용
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			// 해당 함수가 얼마나 걸렸는지 초단위로 체크 가능
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 두가지 분기점
		// 1. AssetManager 가 있으면, AssetManager 의 StreamableManager 를 통해 정적 로딩
		// 2. 아니면, FSoftObjectPath 를 통해 바로 정적 로딩
		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		// StaticLoadObject 를 하기 전에 StaticFindObject 를 통해 확인하고 실패하면 로딩함
		// 매우 느리게 동작해서 마지막 경우의 수에 넣음
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void ULinkAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		// Lock 생성
		// 하나의 스레드만 사용
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
	// Lock 해제
	// LoadedAssetsLock 가 스코프 밖으로 나가며 사라지면서 자연스레 락도 해제된다.
}

void ULinkAssetManager::StartInitialLoading()
{
	// This does all of the scanning, need to do this now even if loads are deferred
	Super::StartInitialLoading();
}

