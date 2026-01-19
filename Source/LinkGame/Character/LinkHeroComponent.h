// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "LinkGame/Input/LinkMappableConfigPair.h"
#include "LinkHeroComponent.generated.h"

struct FInputActionValue;
class ULinkCameraMode;

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class LINKGAME_API ULinkHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	ULinkHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** FeatureName ���� */
	static const FName NAME_ActorFeatureName;

	/**
	 * UPawnComponent interface
	 */
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	/**
	* IGameFrameworkInitStateInterface
	*/
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;
	virtual void CheckDefaultInitialization() final;

	/**
	 * member methods
	 */
	TSubclassOf<ULinkCameraMode> DetermineCameraMode() const;
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);

	/**
	* member variables
	*/
	UPROPERTY(EditAnywhere)
	TArray<FLinkMappableConfigPair> DefaultInputConfigs;
};
