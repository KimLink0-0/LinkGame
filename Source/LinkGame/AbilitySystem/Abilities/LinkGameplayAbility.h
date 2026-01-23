// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LinkGameplayAbility.generated.h"


UCLASS()
class LINKGAME_API ULinkGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	ULinkGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
