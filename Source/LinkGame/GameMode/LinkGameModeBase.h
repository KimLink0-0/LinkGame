// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LinkGameModeBase.generated.h"

class ULinkExperienceDefinition;

UCLASS()
class LINKGAME_API ALinkGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALinkGameModeBase();
	
	// 엔진 실행 극초기에 실행
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	// GameState 가 생성 된 뒤에 실행
	virtual void InitGameState() override;
	
	
	// final 로 하는 이유, 해당 GameMode 를 상속하는 클래스는 없을 예정이라서
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;
	
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;
	
	
	// 멤버 함수 라인
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience);
};
