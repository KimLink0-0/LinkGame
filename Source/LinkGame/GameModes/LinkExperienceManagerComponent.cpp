// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkExperienceManagerComponent.h"
#include "GameFeaturesSubsystemSettings.h"
#include "LinkExperienceDefinition.h"
#include "LinkGame/System/LinkAssetManager.h"

void ULinkExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnLinkExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{

		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void ULinkExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	ULinkAssetManager& AssetManager = ULinkAssetManager::Get();

	TSubclassOf<ULinkExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	const ULinkExperienceDefinition* Experience = GetDefault<ULinkExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void ULinkExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ELinkExperienceLoadState::Unloaded);

	LoadState = ELinkExperienceLoadState::Loading;

	ULinkAssetManager& AssetManager = ULinkAssetManager::Get();
	
	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());
	
	TArray<FName> BundlesToLoad;
	{
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void ULinkExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	OnExperienceFullLoadCompleted();
}

void ULinkExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ELinkExperienceLoadState::Loaded);

	LoadState = ELinkExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const ULinkExperienceDefinition* ULinkExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == ELinkExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
