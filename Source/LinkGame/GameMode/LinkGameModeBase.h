// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LinkGameModeBase.generated.h"


UCLASS()
class LINKGAME_API ALinkGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALinkGameModeBase();
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	void HandleMatchAssignmentIfNotExpectingOne();
};
