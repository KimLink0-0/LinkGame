// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LinkAbilitySystemComponent.h"

#include "Abilities/LinkGameplayAbility.h"
#include "Animation/LinkAnimInstance.h"

ULinkAbilitySystemComponent::ULinkAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULinkAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check (ActorInfo);
	check (InOwnerActor);
	
	const bool bHasNewPawnAvatar = (Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor));
	
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	
	if (bHasNewPawnAvatar)
	{
		if (ULinkAnimInstance* LinkAnimInstance = Cast<ULinkAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			LinkAnimInstance->InitializeWithAbilitySystem(this);
		}
	}
}

void ULinkAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void ULinkAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void ULinkAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	
	// Held abilities
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && AbilitySpec->IsActive())
			{
				const ULinkGameplayAbility* LinkAbilityCDO = CastChecked<ULinkGameplayAbility>(AbilitySpec->Ability);
				
				if (LinkAbilityCDO->ActivationPolicy == ELinkAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}
	
	// Pressed abilities
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const ULinkGameplayAbility* LinkAbilityCDO = CastChecked<ULinkGameplayAbility>(AbilitySpec->Ability);
				
					if (LinkAbilityCDO->ActivationPolicy == ELinkAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}
	
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}
	
	// Release abilities
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}
	
	// Held 는 Released 될 때 제거 했으므로 나머지 두 경우만 리셋
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
