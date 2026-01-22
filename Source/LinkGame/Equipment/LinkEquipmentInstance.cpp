// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkEquipmentInstance.h"

#include "LinkEquipmentDefinition.h"
#include "GameFramework/Character.h"

ULinkEquipmentInstance::ULinkEquipmentInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


APawn* ULinkEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter()); 
}

void ULinkEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void ULinkEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

void ULinkEquipmentInstance::SpawnEquipmentActors(const TArray<FLinkEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Character = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Character->GetMesh();
		}
		
		for (const FLinkEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);
			
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);
			
			SpawnedActors.Add(NewActor);
		}
	}
}

void ULinkEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}
