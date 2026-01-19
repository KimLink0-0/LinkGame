// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LinkGameState.generated.h"


class ULinkExperienceManagerComponent;

UCLASS()
class LINKGAME_API ALinkGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ALinkGameState();
	
public:
	UPROPERTY()
	TObjectPtr<ULinkExperienceManagerComponent> ExperienceManagerComponent;
};
