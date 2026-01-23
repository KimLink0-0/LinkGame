// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LinkEquipmentManagerComponent.h"

#include "LinkEquipmentDefinition.h"
#include "LinkEquipmentInstance.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/LinkAbilitySystemComponent.h"

ULinkEquipmentInstance* FLinkEquipmentList::AddEntry(TSubclassOf<ULinkEquipmentDefinition> EquipmentDefinition)
{
	ULinkEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const ULinkEquipmentDefinition* EquipmentCDO = GetDefault<ULinkEquipmentDefinition>(EquipmentDefinition);
	
	TSubclassOf<ULinkEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = ULinkEquipmentInstance::StaticClass();
	}
	
	FLinkAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<ULinkEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;
	
	// Ability 추가
	ULinkAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	check(AbilitySystemComponent);
	{
		for (const TObjectPtr<ULinkGameplayAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &NewEntry.GrantedHandles, Result);
		}
	}
	
	// Mesh 추가
	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);
	
	return Result;
}

void FLinkEquipmentList::RemoveEntry(ULinkEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FLinkAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			ULinkAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
			check(AbilitySystemComponent);
			
			// Ability 제거 
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
			}
			
			// Mesh 제거
			{
				Instance->DestroyEquipmentActors();
			}
			
			EntryIt.RemoveCurrent();
		}
	}
}

ULinkAbilitySystemComponent* FLinkEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	// EquipmentManagerComponent 의 Pawn 을 가져옴
	AActor* OwningActor = OwnerComponent->GetOwner();
	
	return Cast<ULinkAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

ULinkEquipmentManagerComponent::ULinkEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer) : 
		Super(ObjectInitializer), 
		EquipmentList(this)
{
}

ULinkEquipmentInstance* ULinkEquipmentManagerComponent::EquipItem(TSubclassOf<ULinkEquipmentDefinition> EquipmentDefinition)
{
	ULinkEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}
	return Result;
}

void ULinkEquipmentManagerComponent::UnequipItem(ULinkEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

TArray<ULinkEquipmentInstance*> ULinkEquipmentManagerComponent::GetEquipmentInstanceOfType(
	TSubclassOf<ULinkEquipmentInstance> InstanceType) const
{
	TArray<ULinkEquipmentInstance*> Result;
	
	for (const FLinkAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (ULinkEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Result.Add(Instance);
			}
		}
	}
	
	return Result;
}
