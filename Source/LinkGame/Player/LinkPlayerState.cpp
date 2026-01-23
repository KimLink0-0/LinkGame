// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkPlayerState.h"

#include "AbilitySystem/LinkAbilitySystemComponent.h"
#include "AbilitySystem/LinkGameplayAbilitySet.h"
#include "Character/LinkPawnData.h"
#include "LinkGame/GameModes/LinkExperienceManagerComponent.h"
#include "LinkGame/GameModes/LinkGameModeBase.h"

ALinkPlayerState::ALinkPlayerState(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULinkAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}

void ALinkPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Experience Section
	{
		const AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		ULinkExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULinkExperienceManagerComponent>();
		check(ExperienceManagerComponent);

		ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLinkExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
	
	// Ability System Section
	{
		check(AbilitySystemComponent);
		
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
		
		// (why? 아마 AbilitySystemComponent 의 Owner 가 프로젝트마다 다를 수 있기 때문에 커스텀 가능하게 만든 거 아닐까?)
		// OwnerActor 와 AvatarActor 의 초기 설정은 AbilitySystemComponent 를 생성하는 Actor 이기에 알맞게 재설정하는 과정
		AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
	}
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
	
	// Ability System Section
	for (ULinkGameplayAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}
