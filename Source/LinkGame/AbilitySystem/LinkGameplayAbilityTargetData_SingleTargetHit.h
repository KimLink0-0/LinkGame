// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "LinkGameplayAbilityTargetData_SingleTargetHit.generated.h"

USTRUCT()
struct LINKGAME_API FLinkGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
public:
	FLinkGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{}
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FLinkGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}
	
public:
	UPROPERTY()
	int32 CartridgeID;
};
