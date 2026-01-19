// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkGameModeBase.h"

#include "LinkExperienceDefinition.h"
#include "LinkExperienceManagerComponent.h"
#include "LinkGameState.h"
#include "LinkLogChannels.h"
#include "Character/LinkCharacter.h"
#include "Player/LinkPlayerController.h"
#include "Player/LinkPlayerState.h"
#include "Character/LinkPawnData.h"

ALinkGameModeBase::ALinkGameModeBase()
{
	GameStateClass = ALinkGameState::StaticClass();
	PlayerStateClass = ALinkPlayerState::StaticClass();
	PlayerControllerClass = ALinkPlayerController::StaticClass();
	DefaultPawnClass = ALinkCharacter::StaticClass();
}

void ALinkGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	// InitGame 은 극초반부에 실행되기 때문에, 아직 초기화 작업이 진행 중이므로 Experience 관련 로직을 처리할 수 없음
	// 그래서 한 프레임 뒤에 이벤트를 받아 실행
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALinkGameModeBase::HandleMatchAssignmentIfNotExpectingOne);
}

void ALinkGameModeBase::InitGameState()
{
	Super::InitGameState();
	
	ULinkExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULinkExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLinkExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

const ULinkPawnData* ALinkGameModeBase::GetPawnDataForController(const AController* InController) const
{
	// 게임 도중에 PawnData 가 오버라이드 되었을 경우, PawnData 는 PlayerState 에서 가져오게 됨
	// 라이라 프로젝트의 경우엔 PawnData 는 PlayerState 에서 캐싱하여 사용
	if (InController)
	{
		const ALinkPlayerState* LinkPS = InController->GetPlayerState<ALinkPlayerState>();
		if (LinkPS)
		{
			const ULinkPawnData* PawnData = LinkPS->GetPawnData<ULinkPawnData>();
			if (PawnData)
			{
				return PawnData;
			}
		}
	}
	
	// 캐시가 없을 경우엔 안정성 검사를 해서 가져오게 됨 
	// 초기에는 캐싱된 PawnData 가 없기 때문에 한 번은 실행 됨
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

void ALinkGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);	
	}
}

// APawn* ALinkGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
// 	const FTransform& SpawnTransform)
// {
// 	UE_LOG(LogLink, Log, TEXT("SpawnDefaultPawnAtTransform_Implementation is called!"));
// 	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
// }

UClass* ALinkGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// Pawn Data 를 가져오는 로직은 PlayerState 에서든 GameMode 에서든 GetPawnDataForController 를 사용
	if (const ULinkPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ALinkGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	// OnMatchAssignmentGiven 으로 넘겨줄 로컬 변수 선언
	FPrimaryAssetId ExperienceId;
	
	
	// UWorld* World = GetWorld();
	
	// 이 함수가 완료됐다 != 로딩이 완료됐다
	// Default experience
	// 라이라 프로젝트엔 많은 경우의 수가 있어서 그걸 매칭해보지만 우린 일단 디폴트 값을 매칭
	
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
	// PlayerController 를 순회해서 
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		
		// PlayerController 가 Pawn 에 Possess 하지 않았다면, RestartPlayer 를 통해 Pawn 을 다시 Spawn 한다
		// RestartPlayer 로직을 타고 들어가면 Pawn 설정부터 Possess 까지의 과정이 진행된다 
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}
