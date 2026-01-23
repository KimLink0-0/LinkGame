// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkCosmeticAnimationTypes.h"

TSubclassOf<UAnimInstance> FLinkAnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FLinkAnimLayerSelectionEntry& Rule : LayerRules)
	{
		if (Rule.Layer != nullptr && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Layer;
		}
	}
	
	return DefaultLayer;
}

USkeletalMesh* FLinkAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FLinkAnimBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if (Rule.Mesh && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Mesh;
		}
	}
	
	return DefaultMesh;
}
