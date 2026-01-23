// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LinkGameplayAbility.h"

ULinkGameplayAbility::ULinkGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = ELinkAbilityActivationPolicy::OnInputTriggered;
}
