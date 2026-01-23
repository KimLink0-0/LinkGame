// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LinkAbilitySystemComponent.generated.h"


UCLASS()
class LINKGAME_API ULinkAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	ULinkAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
