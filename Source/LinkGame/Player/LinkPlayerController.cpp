// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkPlayerController.h"

#include "LinkPlayerState.h"
#include "AbilitySystem/LinkAbilitySystemComponent.h"
#include "Camera/LinkPlayerCameraManager.h"

ALinkPlayerController::ALinkPlayerController(const FObjectInitializer& ObjectInitializer)
{
	PlayerCameraManagerClass = ALinkPlayerCameraManager::StaticClass();
}

void ALinkPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (ULinkAbilitySystemComponent* LinkASC = GetLinkAbilitySystemComponent())
	{
		LinkASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

ALinkPlayerState* ALinkPlayerController::GetLinkPlayerState() const
{
	return CastChecked<ALinkPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

ULinkAbilitySystemComponent* ALinkPlayerController::GetLinkAbilitySystemComponent() const
{
	const ALinkPlayerState* LinkPS = GetLinkPlayerState();
	return LinkPS ? LinkPS->GetLinkAbilitySystemComponent() : nullptr;
}
