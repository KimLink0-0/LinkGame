// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LinkEquipmentInstance.generated.h"

struct FLinkEquipmentActorToSpawn;

UCLASS()
class LINKGAME_API ULinkEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	ULinkEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();
	
	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();
	
	UFUNCTION(BlueprintPure, Category = Equipment)
	APawn* GetPawn() const;
	
	virtual void OnEquipped();
	
	virtual void OnUnequipped();
	
	void SpawnEquipmentActors(const TArray<FLinkEquipmentActorToSpawn>& ActorsToSpawn);
	void DestroyEquipmentActors();
	
public:
	// 누구에 의해서 활성화가 되었는지? 이게 왜 필요할까
	UPROPERTY()
	TObjectPtr<UObject> Instigator;
	
	// 장착된 아이템이 변경 혹은 해제 됐을 때 전부 파괴해야 하기 때문에 Array 로 관리
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
