// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkExperienceManagerComponent.h"

#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeaturesSubsystemSettings.h"
#include "LinkExperienceActionSet.h"
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
	// 게임 Feature 를 Load 하고 Activate 하는 함수 
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	check(LoadState == ELinkExperienceLoadState::Loading);
	check(CurrentExperience);
	
	// 이전 활성화 된 GameFeature Plugin 의 URL 을 Clear 
	GameFeaturePluginURLs.Reset();
	
	// 람다식 사용법 익히기
	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
	{
		for (const FString& PluginName : FeaturePluginList)
		{
			FString PluginURL;
			if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
			{
				This->GameFeaturePluginURLs.AddUnique(PluginURL);
			}
		}
	};
	
	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);
	
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadState = ELinkExperienceLoadState::LadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		OnExperienceFullLoadCompleted();
	}
}

void ULinkExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		OnExperienceFullLoadCompleted();
	}
}

void ULinkExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ELinkExperienceLoadState::Loaded);

	LoadState = ELinkExperienceLoadState::ExecutingActions;
	
	FGameFeatureActivatingContext Context;
	
	// Context 에 WorldContextHandle 을 저장시켜주기 위한 로직
	// WorldContextFormWorld 가 왜 필요한가? 
	// Game Feature 는 Game Feature Subsystem 에서 관리, 해당 Subsystem 은 EngineSubsystem
	// WorldContext 가 없으면 어떤 월드의 Action 이 실행되는지 알 수 없음
	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
	if (ExistingWorldContext)
	{
		Context.SetRequiredWorldContextHandle((ExistingWorldContext->ContextHandle));
	}
	
	
	auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
	{
		for (UGameFeatureAction* Action : ActionList)
		{
			if (Action)
			{
				// GameFeature 는 Registering -> Loading -> Activating 순으로 진행된다. 
				Action->OnGameFeatureRegistering();
				Action->OnGameFeatureLoading();
				Action->OnGameFeatureActivating(Context);
			}
		}
	};
	
	ActivateListOfActions(CurrentExperience->Actions);
	
	for (const TObjectPtr<ULinkExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	{
		ActivateListOfActions(ActionSet->Actions);
	}
	
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


