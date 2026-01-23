// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LinkPawnData.generated.h"

class ULinkGameplayAbilitySet;
class ULinkInputConfig;
class ULinkCameraMode;

UCLASS()
class LINKGAME_API ULinkPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	ULinkPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Pawn Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Link|Pawn")
	TSubclassOf<APawn> PawnClass;

	/** Camera Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Link|Camera")
	TSubclassOf<ULinkCameraMode> DefaultCameraMode;
	
	/** input configuration used by player controlled pawns to create input mappings and bind input actions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Link|InputConfig")
	TObjectPtr<ULinkInputConfig> InputConfig;
	
	/** Ability Sets */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Link|Abilities")
	TArray<TObjectPtr<ULinkGameplayAbilitySet>> AbilitySets;
};
