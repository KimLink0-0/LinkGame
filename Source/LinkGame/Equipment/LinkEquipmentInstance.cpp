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

APawn* ULinkEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		// 정의의 DeterminesOutputType 에 의해 형 변환이 될 거라
		// 해당 Type 이 유효한지 체크
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
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
