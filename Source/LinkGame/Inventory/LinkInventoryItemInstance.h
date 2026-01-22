// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LinkInventoryItemInstance.generated.h"


class ULinkInventoryItemDefinition;
class ULinkInventoryItemFragment;

UCLASS()
class LINKGAME_API ULinkInventoryItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	ULinkInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	const ULinkInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULinkInventoryItemFragment> FragmentClass) const;
	
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}
	
public:
	UPROPERTY()
	TSubclassOf<ULinkInventoryItemDefinition> ItemDefinition;
};
