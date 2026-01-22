// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkInventoryManagerComponent.h"

ULinkInventoryManagerComponent::ULinkInventoryManagerComponent(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer),
	InventoryList(this)
{
}
