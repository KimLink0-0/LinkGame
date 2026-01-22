// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LinkInventoryItemInstance.generated.h"


class ULinkInventoryItemDefinition;

UCLASS()
class LINKGAME_API ULinkInventoryItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	ULinkInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY()
	TSubclassOf<ULinkInventoryItemDefinition> ItemDefinition;
};
