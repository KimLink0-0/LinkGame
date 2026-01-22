// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LinkInventoryItemInstance.h"

#include "LinkInventoryItemDefinition.h"

ULinkInventoryItemInstance::ULinkInventoryItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const ULinkInventoryItemFragment* ULinkInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<ULinkInventoryItemFragment> FragmentClass) const
{
	if (ItemDefinition != nullptr && FragmentClass != nullptr)
	{
		// EditNewLine, Instance 등의 메타 키워드를 통해 Fragment 를 수정했으므로 CDO 로 가져와도 수정 사항이 반영이 된다.
		return GetDefault<ULinkInventoryItemDefinition>(ItemDefinition)->FindFragmentByClass(FragmentClass);
	}
	
	return nullptr;
}
