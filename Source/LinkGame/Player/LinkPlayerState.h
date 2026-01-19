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

	virtual void PostInitializeComponents() final;


	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void OnExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience);
	void SetPawnData(const ULinkPawnData* InPawnData);

	UPROPERTY()
	TObjectPtr<const ULinkPawnData> PawnData;
};
