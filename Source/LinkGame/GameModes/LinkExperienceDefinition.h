// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LinkExperienceDefinition.generated.h"

class ULinkPawnData;
class ULinkExperienceActionSet;
class UGameFeatureAction;

UCLASS()
class LINKGAME_API ULinkExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	ULinkExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
public:
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<ULinkPawnData> DefaultPawnData;
	
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;
	
	// Gameplay 용도에 맞게 분류의 목적으로 사용 일종의 Filter. 
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<ULinkExperienceActionSet>> ActionSets;
	
	// GameFeatureAction 리스트
	UPROPERTY(EditDefaultsOnly, Category= "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
