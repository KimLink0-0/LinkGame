// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LinkGameModeBase.generated.h"


class ULinkExperienceDefinition;
class ULinkPawnData;

UCLASS()
class LINKGAME_API ALinkGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ALinkGameModeBase();
	
	// AGameModeBase Interface 
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	
	// Member Methods
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience);
	const ULinkPawnData* GetPawnDataForController(const AController* InController) const;
};
