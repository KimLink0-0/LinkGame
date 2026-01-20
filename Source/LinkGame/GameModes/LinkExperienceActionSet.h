// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LinkExperienceActionSet.generated.h"

class UGameFeatureAction;

UCLASS()
class LINKGAME_API ULinkExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	ULinkExperienceActionSet();
	
	UPROPERTY(EditAnywhere, Category = "Action to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
