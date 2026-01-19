// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LinkExperienceDefinition.generated.h"

class ULinkPawnData;


UCLASS()
class LINKGAME_API ULinkExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	ULinkExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<ULinkPawnData> DefaultPawnData;
	
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;
};
