// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkPlayerState.h"

#include "GameMode/LinkExperienceManagerComponent.h"

void ALinkPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);
	
	ULinkExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULinkExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLinkExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnLinkExperienceLoaded));
}

void ALinkPlayerState::OnLinkExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience)
{
}
