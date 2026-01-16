// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LinkPlayerState.generated.h"


class ULinkPawnData;
class ULinkExperienceDefinition;

UCLASS()
class LINKGAME_API ALinkPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	template<typename T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	
	void SetPawnData(const ULinkPawnData* InPawnData);
	
	virtual void PostInitializeComponents() override;
	
	void OnLinkExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience);
	
public:
	UPROPERTY()
	TObjectPtr<const ULinkPawnData> PawnData;
};
