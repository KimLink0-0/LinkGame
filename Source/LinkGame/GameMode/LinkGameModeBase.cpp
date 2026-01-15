// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkGameModeBase.h"
#include "LinkGameState.h"
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

void ALinkGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	
}
