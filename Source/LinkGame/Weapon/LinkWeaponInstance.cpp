// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LinkWeaponInstance.h"

ULinkWeaponInstance::ULinkWeaponInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

TSubclassOf<UAnimInstance> ULinkWeaponInstance::PickBestAnimLayer(bool bEquipped,
	const FGameplayTagContainer& CosmeticTags) const
{
	const FLinkAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}
