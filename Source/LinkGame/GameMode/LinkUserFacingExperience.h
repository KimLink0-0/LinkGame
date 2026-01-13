// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LinkUserFacingExperience.generated.h"


UCLASS()
class LINKGAME_API ULinkUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowTypes = "Map"))
	FPrimaryAssetId MapID;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowTypes = "LinkExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
	
	
};
