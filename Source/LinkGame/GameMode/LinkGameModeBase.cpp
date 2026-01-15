// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkGameModeBase.h"

#include "LinkExperienceManagerComponent.h"
#include "LinkGameState.h"
#include "LinkLogChannels.h"
#include "Character/LinkCharacter.h"
#include "Player/LinkPlayerController.h"
#include "Player/LinkPlayerState.h"

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

void ALinkGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);	
	}
}

APawn* ALinkGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	UE_LOG(LogLink, Log, TEXT("SpawnDefaultPawnAtTransform_Implementation is called!"));
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void ALinkGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	// OnMatchAssignmentGiven 으로 넘겨줄 로컬 변수 선언
	FPrimaryAssetId ExperienceId;
	
	
	UWorld* World = GetWorld();
	
	// 이 함수가 완료됐다 != 로딩이 완료됐다
	// Default experience
	// 라이라 프로젝트엔 많은 경우의 수가 있어서 그걸 매칭해보지만 우린 일단 디폴트 값을 매칭
	
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("LinkExperienceDefinition"), FName("B_LinkExperienceDefinition"));
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
}
