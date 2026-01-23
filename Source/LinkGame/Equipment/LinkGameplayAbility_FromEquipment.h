// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LinkGameplayAbility.h"
#include "LinkGameplayAbility_FromEquipment.generated.h"


class ULinkEquipmentInstance;

UCLASS()
class LINKGAME_API ULinkGameplayAbility_FromEquipment : public ULinkGameplayAbility
{
	GENERATED_BODY()
public:
	ULinkEquipmentInstance* GetAssociatedEquipment() const;
};
