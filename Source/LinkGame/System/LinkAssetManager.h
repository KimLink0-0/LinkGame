// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LinkAssetManager.generated.h"

UCLASS()
class LINKGAME_API ULinkAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	ULinkAssetManager();
	
	static ULinkAssetManager& Get();
	
	// Returns the asset referenced by a TSoftObjectPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// Returns the subclass referenced by a TSoftClassPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	
protected:
	static bool ShouldLogAssetLoads();
	
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	
	void AddLoadedAsset(const UObject* Asset);
	
	virtual void StartInitialLoading() override;
	
	
private:
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// Used for a scope lock when modifying the list of load assets.
	// Object 단위 Locking
	FCriticalSection LoadedAssetsCritical;
	
};

// GetAsset은 정적 로딩으로 BP Class 와 Object 로딩에 사용
// AssetManager 는 GetAsset 또는 GetSubclass 를 통해서 에셋을 동기 로딩을 진행한다
template<typename AssetType>
AssetType* ULinkAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		// 로딩이 되어 있을 경우 -> 바로 가져옴
		LoadedAsset = AssetPointer.Get();
		
		// 로딩이 안되어 있을 경우 -> Null 이므로 로딩을 시작함
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			// AddLoadAsset 은 메모리에 상주하기 위한 장치
			// 한 번 등록되면 직접 내리지 않는 한 Unload 가 되지 않음 (== 캐싱)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
TSubclassOf<AssetType> ULinkAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}