// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "LinkPlayerController.generated.h"

class ALinkPlayerState;
class ULinkAbilitySystemComponent;

UCLASS

()
class LINKGAME_API ALinkPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ALinkPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// PlayerController's Interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
	// Member Methods 
	ALinkPlayerState* GetLinkPlayerState() const;
	ULinkAbilitySystemComponent* GetLinkAbilitySystemComponent() const;
};
