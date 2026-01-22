// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LinkEquipmentManagerComponent.h"

#include "LinkEquipmentDefinition.h"
#include "LinkEquipmentInstance.h"

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
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
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
