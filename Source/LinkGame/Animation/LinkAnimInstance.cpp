// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LinkAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void ULinkAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* AbilitySystemComp = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(AbilitySystemComp);
		}
	}
}

void ULinkAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent)
{
	// GameplayTag 와 AnimInstance 의 멤버 Property 와 Delegate 연결, 값 변화에 대한 반영을 진행
	GameplayTagPropertyMap.Initialize(this, AbilitySystemComponent);
}
