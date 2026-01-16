// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkPlayerState.h"

#include "GameMode/LinkExperienceManagerComponent.h"
#include "GameMode/LinkGameModeBase.h"


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
	if (ALinkGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALinkGameModeBase>())
	{
		// - 아직 PawnData 를 설정하지 않았으므로, ExperienceManagerComponent 에서 DefaultPawnData 로 설정
		// GetPawnDataForController 는 PawnData 를 가져오는 공용 함수로 다른 곳에서도 사용할 예정 (재사용성 증가)
		const ULinkPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);
		
		SetPawnData(NewPawnData);
	}
}

void ALinkPlayerState::SetPawnData(const ULinkPawnData* InPawnData)
{
	check(InPawnData);
	
	// PawnData 가 두번 설정되는 것을 원하지 않음!
	check(!PawnData);
	
	PawnData = InPawnData;
}
