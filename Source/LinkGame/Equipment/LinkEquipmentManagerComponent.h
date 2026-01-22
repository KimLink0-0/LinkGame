// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "LinkEquipmentManagerComponent.generated.h"

class ULinkEquipmentInstance;
class ULinkEquipmentDefinition;

USTRUCT(BlueprintType)
struct FLinkAppliedEquipmentEntry
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TSubclassOf<ULinkEquipmentDefinition> EquipmentDefinition;
	
	UPROPERTY()
	TObjectPtr<ULinkEquipmentInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FLinkEquipmentList
{
	GENERATED_BODY()
	
public:
	FLinkEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}
	
	ULinkEquipmentInstance* AddEntry(TSubclassOf<ULinkEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(ULinkEquipmentInstance* Instance);
	
public:
	UPROPERTY()
	TArray<FLinkAppliedEquipmentEntry> Entries;
	
	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LINKGAME_API ULinkEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	ULinkEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	ULinkEquipmentInstance* EquipItem(TSubclassOf<ULinkEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(ULinkEquipmentInstance* ItemInstance);
	
public:
	UPROPERTY()
	FLinkEquipmentList EquipmentList;
};
