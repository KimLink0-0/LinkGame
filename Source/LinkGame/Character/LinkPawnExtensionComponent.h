// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "LinkPawnExtensionComponent.generated.h"

class ULinkPawnData;

UCLASS()
class LINKGAME_API ULinkPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	ULinkPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;
	
	static ULinkPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULinkPawnExtensionComponent>() : nullptr); }
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const ULinkPawnData* InPawnData);
	void SetupPlayerInputComponent();
	
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;
	
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;
	
	UPROPERTY(EditInstanceOnly, Category = "Link|Pawn")
	TObjectPtr<const ULinkPawnData> PawnData;
};
