// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkEquipmentDefinition.h"

#include "LinkEquipmentInstance.h"

ULinkEquipmentDefinition::ULinkEquipmentDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstanceType = ULinkEquipmentInstance::StaticClass();
}
