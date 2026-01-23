// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LinkGameplayAbilitySet.h"

#include "LinkAbilitySystemComponent.h"
#include "Abilities/LinkGameplayAbility.h"

void FLinkAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FLinkAbilitySet_GrantedHandles::TakeFromAbilitySystem(ULinkAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		return;
	}
	
	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			InAbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

ULinkGameplayAbilitySet::ULinkGameplayAbilitySet(const FObjectInitializer& Initializer) : Super(Initializer)
{
}

void ULinkGameplayAbilitySet::GiveToAbilitySystem(ULinkAbilitySystemComponent* AbilitySystemComponent,
	FLinkAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(AbilitySystemComponent);
	
	if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		return;
	}
	
	// 시작
	{
		for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
		{
			const FLinkAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
			if (!IsValid(AbilityToGrant.Ability))
			{
				continue;
			}
			
			// GiveAbility 에서 만약 EGameplayAbilityInstancingPolicy::InstancedPerActor 라면, 내부적으로 Instance 를 생성하지만
			// 라이라는 기본값(CDO)으로만 사용
			// 이를 통해 UObject 갯수를 줄여 성능적/메모리적 부하를 줄일 수 있음
			// Ability 의 수치가 달라져야 한다면, InstancePerActor 을 사용 (일반적)
			{
				ULinkGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<ULinkGameplayAbility>();
			
				FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
				AbilitySpec.SourceObject = SourceObject;
				AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);
			
				// 능력 부여, 배열로 Spec 를 Handle 에 맵핑
				const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
				if (OutGrantedHandles)
				{
					OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
				}
			}
		}
	}
}
