// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "LinkQuickBarComponent.generated.h"

class ULinkEquipmentInstance;
class ULinkInventoryItemInstance;
class ULinkEquipmentManagerComponent;

// Lyra 는 FPS 게임이다 보니까 거기에 맞는 HUD 를 위한 Component 가 존재
// Inventory 와 Equipment 의 중간 다리 역할을 하게 되는 Component 이다

UCLASS()
class LINKGAME_API ULinkQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
public:
	ULinkQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// Member Methods
	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, ULinkInventoryItemInstance* Item);
	UFUNCTION(BlueprintCallable)
	void SetActiveSlotIndex(int32 NewIndex);
	ULinkEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();
	
	
	// ControllerComponent's Interface
	virtual void BeginPlay() override;
	
public:
	// 퀵바에 등록할 수 있는 아이템의 갯수
	UPROPERTY()
	int32 NumSlots = 3;
	
	// 퀵바에 등록된 아이템 목록
	// RPG 의 장비창 
	// FPS 의 1~5 버튼창 정도로 생각하면 될 듯
	UPROPERTY()
	TArray<TObjectPtr<ULinkInventoryItemInstance>> Slots;
	
	// FPS 의 경우 무기를 하나씩 드는 경우가 많다 보니까 Active Slot 이 하나만 존재
	UPROPERTY()
	int32 ActiveSlotIndex = -1;
	
	UPROPERTY()
	TObjectPtr<ULinkEquipmentInstance> EquippedItem;
};
