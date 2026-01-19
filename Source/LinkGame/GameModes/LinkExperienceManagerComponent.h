// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LinkExperienceManagerComponent.generated.h"

class ULinkExperienceDefinition;

enum class ELinkExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLinkExperienceLoaded, const ULinkExperienceDefinition*);

UCLASS()
class LINKGAME_API ULinkExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	bool IsExperienceLoaded() { return (LoadState == ELinkExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }
	
	void CallOrRegister_OnExperienceLoaded(FOnLinkExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadCompleted();
	const ULinkExperienceDefinition* GetCurrentExperienceChecked() const;

public:
	UPROPERTY()
	TObjectPtr<const ULinkExperienceDefinition> CurrentExperience;

	ELinkExperienceLoadState LoadState = ELinkExperienceLoadState::Unloaded;

	FOnLinkExperienceLoaded OnExperienceLoaded;
};
