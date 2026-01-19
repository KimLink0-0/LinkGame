// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkGameModeBase.h"

#include "LinkExperienceDefinition.h"
#include "LinkExperienceManagerComponent.h"
#include "LinkGameState.h"
#include "Character/LinkCharacter.h"
#include "Player/LinkPlayerController.h"
#include "Player/LinkPlayerState.h"
#include "Character/LinkPawnData.h"
#include "Character/LinkPawnExtensionComponent.h"

ALinkGameModeBase::ALinkGameModeBase()
{
	GameStateClass = ALinkGameState::StaticClass();
	PlayerControllerClass = ALinkPlayerController::StaticClass();
	PlayerStateClass = ALinkPlayerState::StaticClass();
	DefaultPawnClass = ALinkCharacter::StaticClass();
}

void ALinkGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ALinkGameModeBase::InitGameState()
{
	Super::InitGameState();

	ULinkExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULinkExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLinkExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ALinkGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const ULinkPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ALinkGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* ALinkGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (ULinkPawnExtensionComponent* PawnExtComp = ULinkPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const ULinkPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
}

void ALinkGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;

	UWorld* World = GetWorld();
	
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("LinkExperienceDefinition"), FName("B_LinkDefaultExperience"));
	}
	
	OnMatchAssignmentGiven(ExperienceId);
}

void ALinkGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	check(ExperienceId.IsValid());

	ULinkExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULinkExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

bool ALinkGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	ULinkExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULinkExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ALinkGameModeBase::OnExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);


		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const ULinkPawnData* ALinkGameModeBase::GetPawnDataForController(const AController* InController) const
{
	if (InController)
	{
		if (const ALinkPlayerState* LinkPS = InController->GetPlayerState<ALinkPlayerState>())
		{
			// GetPawnData 
			if (const ULinkPawnData* PawnData = LinkPS->GetPawnData<ULinkPawnData>())
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	ULinkExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULinkExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const ULinkExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	return nullptr;
}



