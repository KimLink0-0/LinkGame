// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LinkInventoryItemDefinition.h"

ULinkInventoryItemDefinition::ULinkInventoryItemDefinition(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
}

const ULinkInventoryItemFragment* ULinkInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<ULinkInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (ULinkInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}
