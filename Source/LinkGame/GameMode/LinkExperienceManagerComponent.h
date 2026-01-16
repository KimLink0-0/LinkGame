// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LinkExperienceManagerComponent.generated.h"

enum class ELinkExperienceLoadState
{
	Unloaded,
	Loaded,
	Loading,
	Deactivating,
};

class ULinkExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLinkExperienceLoaded, const ULinkExperienceDefinition*)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LINKGAME_API ULinkExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULinkExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	
	void CallOrRegister_OnExperienceLoaded(FOnLinkExperienceLoaded::FDelegate&& Delegate);
	
	const ULinkExperienceDefinition* GetCurrentExperienceChecked() const;
	
	// Returns true if the experience is fully loaded
	bool IsExperienceLoaded() const;
	
private:
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadCompleted();

private:
	FOnLinkExperienceLoaded OnExperienceLoaded;
	
	ELinkExperienceLoadState LoadState = ELinkExperienceLoadState::Unloaded;
	
	UPROPERTY()
	TObjectPtr<const ULinkExperienceDefinition> CurrentExperience;
	
};
