// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LinkExperienceDefinition.generated.h"

class ULinkPawnData;

UCLASS()
class LINKGAME_API ULinkExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULinkExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<ULinkPawnData> DefaultPawnData;
	
	// 마킹 및 기억용
	// 게임 모드에 따른 GameFeature plugin 을 로딩하는데 필요한 연결고리
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;
	
};
