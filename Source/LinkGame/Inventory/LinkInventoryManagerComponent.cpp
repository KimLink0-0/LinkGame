// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkInventoryManagerComponent.h"

#include "LinkInventoryItemDefinition.h"
#include "LinkInventoryItemInstance.h"

ULinkInventoryItemInstance* FLinkInventoryList::AddEntry(TSubclassOf<ULinkInventoryItemDefinition> ItemDef)
{
	ULinkInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);
	
	// OwningActor = PlayerController 
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	
	FLinkInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<ULinkInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDefinition = ItemDef;
	
	Result = NewEntry.Instance;
	return Result;
}

ULinkInventoryManagerComponent::ULinkInventoryManagerComponent(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer),
	InventoryList(this)
{
}

ULinkInventoryItemInstance* ULinkInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<ULinkInventoryItemDefinition> ItemDef)
{
	ULinkInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}
	return Result;
}
