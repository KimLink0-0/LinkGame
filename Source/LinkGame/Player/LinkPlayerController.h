// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LinkPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LINKGAME_API ALinkPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ALinkPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
