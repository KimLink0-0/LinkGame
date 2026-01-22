// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LinkInventoryItemDefinition.h"
#include "LinkInventoryFragment_EquippableItem.generated.h"

class ULinkEquipmentDefinition;

UCLASS()
class LINKGAME_API ULinkInventoryFragment_EquippableItem : public ULinkInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Link)
	TSubclassOf<ULinkEquipmentDefinition> EquipmentDefinition;
};
