// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LinkInventoryManagerComponent.generated.h"

class ULinkInventoryItemDefinition;
class ULinkInventoryItemInstance;

USTRUCT(BlueprintType)
struct FLinkInventoryEntry
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<ULinkInventoryItemInstance> Instance = nullptr;
};


USTRUCT(BlueprintType)
struct FLinkInventoryList
{
	GENERATED_BODY()
public:
	FLinkInventoryList(UActorComponent* InOwnerComponent = nullptr) 
		: OwnerComponent(InOwnerComponent)
	{}
	
	UPROPERTY()
	TArray<FLinkInventoryEntry> Entries;
	
	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
	
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LINKGAME_API ULinkInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	ULinkInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY()
	FLinkInventoryList InventoryList;
};
