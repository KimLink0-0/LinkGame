// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkAssetManager.h"
#include "LinkGame/LinkGameplayTags.h"
#include "LinkGame/LinkLogChannels.h"

ULinkAssetManager::ULinkAssetManager()
{
}

ULinkAssetManager& ULinkAssetManager::Get()
{
	check(GEngine);

	if (ULinkAssetManager* Singleton = Cast<ULinkAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogLink, Fatal, TEXT("invalid AssetManagerClassname in DefaultEngine.ini(project settings); it must be LinkAssetManager"));

	return *NewObject<ULinkAssetManager>();
}

void ULinkAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FLinkGameplayTags::InitializeNativeTags();
}

bool ULinkAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

UObject* ULinkAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

	
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void ULinkAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}


