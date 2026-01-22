// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/LinkEquipmentInstance.h"
#include "Cosmetics/LinkCosmeticAnimationTypes.h"
#include "LinkWeaponInstance.generated.h"



UCLASS()
class LINKGAME_API ULinkWeaponInstance : public ULinkEquipmentInstance
{
	GENERATED_BODY()
public:
	ULinkWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLinkAnimLayerSelectionSet EquippedAnimSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLinkAnimLayerSelectionSet UnequippedAnimSet;
};
