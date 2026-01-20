// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LinkUserFacingExperience.generated.h"

class UCommonSession_HostSessionRequest;
/**
 * 
 */
UCLASS()
class LINKGAME_API ULinkUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Plugin CommonUser
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;
	
	/** the specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	/** the gameplay expierence to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "LinkExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};
