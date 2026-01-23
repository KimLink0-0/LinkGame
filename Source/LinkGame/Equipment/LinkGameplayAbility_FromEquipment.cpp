// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkGameplayAbility_FromEquipment.h"

#include "LinkEquipmentInstance.h"

ULinkEquipmentInstance* ULinkGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<ULinkEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}
