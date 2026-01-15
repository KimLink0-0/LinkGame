// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LinkPlayerState.generated.h"


class ULinkExperienceDefinition;

UCLASS()
class LINKGAME_API ALinkPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;
	
	void OnLinkExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience);
};
