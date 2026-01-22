// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LinkQuickBarComponent.h"

#include "LinkEquipmentDefinition.h"
#include "LinkEquipmentInstance.h"
#include "LinkEquipmentManagerComponent.h"
#include "LinkLogChannels.h"
#include "Inventory/LinkInventoryFragment_EquippableItem.h"
#include "Inventory/LinkInventoryItemInstance.h"

ULinkQuickBarComponent::ULinkQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULinkQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
	
	Super::BeginPlay();
}

void ULinkQuickBarComponent::AddItemToSlot(int32 SlotIndex, ULinkInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void ULinkQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		// 경우의 수를 많이 커버 해주는 패턴, 제거 후 등록
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}

ULinkEquipmentManagerComponent* ULinkQuickBarComponent::FindEquipmentManager() const
{

	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<ULinkEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void ULinkQuickBarComponent::UnequipItemInSlot()
{
	if (ULinkEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem)
		{
			EquipmentManager->UnequipItem(EquippedItem);

			EquippedItem = nullptr;
		}
	}
}

void ULinkQuickBarComponent::EquipItemInSlot()
{
	
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	if (ULinkInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const ULinkInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<ULinkInventoryFragment_EquippableItem>())
		{
			TSubclassOf<ULinkEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				
				if (ULinkEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);

					if (EquippedItem)
					{
						EquippedItem->Instigator = SlotItem;
					}
				}
			}
		}
	}
}