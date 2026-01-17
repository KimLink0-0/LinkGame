// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "LinkHeroComponent.generated.h"


UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class LINKGAME_API ULinkHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	ULinkHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// FeatureName 정의
	static const FName NAME_ActorFeatureName;
	
	virtual FName GetFeatureName() const override final { return NAME_ActorFeatureName; }
	
	
	// IGameFrameworkInitStateInterface
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;
	
	virtual void CheckDefaultInitialization() final;
	
	
	// Lifecycle 
	void OnRegister() final;
	
	void BeginPlay() final;
	
	void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

};
