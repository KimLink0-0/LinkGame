// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkPlayerState.h"
#include "LinkGame/GameModes/LinkExperienceManagerComponent.h"
#include "LinkGame/GameModes/LinkGameModeBase.h"

void ALinkPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	ULinkExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULinkExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLinkExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ALinkPlayerState::OnExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience)
{
	if (ALinkGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALinkGameModeBase>())
	{
		const ULinkPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ALinkPlayerState::SetPawnData(const ULinkPawnData* InPawnData)
{
	check(InPawnData);

	check(!PawnData);

	PawnData = InPawnData;
}
