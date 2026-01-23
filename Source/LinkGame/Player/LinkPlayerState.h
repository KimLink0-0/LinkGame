// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LinkPlayerState.generated.h"

class ULinkPawnData;
class ULinkExperienceDefinition;
class ULinkAbilitySystemComponent;

UCLASS()
class LINKGAME_API ALinkPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ALinkPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// APlayerState's Interface
	virtual void PostInitializeComponents() final;

	// Member Methods
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	
	void SetPawnData(const ULinkPawnData* InPawnData);
	ULinkAbilitySystemComponent* GetLinkAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	void OnExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience);

public:
	// Experience 와 GameFeature 를 위한 PawnData 캐싱
	UPROPERTY()
	TObjectPtr<const ULinkPawnData> PawnData;
	
	// Gameplay Ability 를 사용하기 위해서 생성하고 캐싱
	UPROPERTY(VisibleAnywhere, Category= "Link|PlayerState")
	TObjectPtr<ULinkAbilitySystemComponent> AbilitySystemComponent;
};
