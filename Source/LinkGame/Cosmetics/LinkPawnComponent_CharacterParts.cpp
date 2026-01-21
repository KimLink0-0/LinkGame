// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkPawnComponent_CharacterParts.h"

#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"

bool FLinkCharacterPartList::SpawnActorForEntry(FLinkAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActor = false;
	
	if (Entry.Part.PartClass != nullptr)
	{
		UWorld* World = OwnerComponent->GetWorld();
		
		if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
		{
			const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);
			
			UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
			PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
			PartComponent->SetChildActorClass(Entry.Part.PartClass);
			// RegisterComponent 를 통해서 RenderWorld 인 FScene 에 변경 내용이 반영되도록 함
			// 월드는 데이터를 관리하는 월드와 렌더를 관리하는 월드가 따로 존재. 멀티 스레드로 계산
			PartComponent->RegisterComponent();
			
			if (AActor* SpawnedActor = PartComponent->GetChildActor())
			{
				if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
				{
					// SpawnedActor 가 있는 Component 가 OwnerActor 보다 먼저 Tick 이 실행되지 않도록 조건 부여
					// OwnerActor 의 SceneComponent 다음으로 Spawned 된 Actor 의 RootComponent 의 Tick 이 실행 된다.
					SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
				}
			}
			Entry.SpawnedComponent = PartComponent;
			bCreatedAnyActor = true;
		}
	}
	
	return bCreatedAnyActor;
}

void FLinkCharacterPartList::DestroyActorForEntry(FLinkAppliedCharacterPartEntry& Entry)
{
	if (Entry.SpawnedComponent)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
	}
}

FGameplayTagContainer FLinkCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;
	
	for (const FLinkAppliedCharacterPartEntry& Entry : Entries)
	{
		if (Entry.SpawnedComponent)
		{
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent->GetChildActor()))
			{
				TagInterface->GetOwnedGameplayTags(Result);
			}
		}
	}
	
	return Result;
}

FLinkCharacterPartHandle FLinkCharacterPartList::AddEntry(FLinkCharacterPart NewPart)
{
	FLinkCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;
	
	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		FLinkAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart;
		NewEntry.PartHandle = Result.PartHandle;
		
		if (SpawnActorForEntry(NewEntry))
		{
			OwnerComponent->BroadcastChanged();
		}
	}
	
	return Result;
}

void FLinkCharacterPartList::RemoveEntry(FLinkCharacterPartHandle Handle)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FLinkAppliedCharacterPartEntry& Entry = *EntryIt;
		
		if (Entry.PartHandle == Handle.PartHandle)
		{
			DestroyActorForEntry(Entry);
		}
	}
}

ULinkPawnComponent_CharacterParts::ULinkPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : 
	Super (ObjectInitializer),
	CharacterPartList(this)
{
}

USkeletalMeshComponent* ULinkPawnComponent_CharacterParts::GetParentMeshComponent() const
{
	if (AActor* OwnerActor = GetOwner())
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh())
			{
				return MeshComponent;
			}
		}
	}
	
	return nullptr;
}

USceneComponent* ULinkPawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		return MeshComponent;
	}
	else if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->GetRootComponent();
	}
	
	return nullptr;
}

FGameplayTagContainer ULinkPawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}
	else
	{
		return Result;
	}
}

void ULinkPawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReInitPose = true;
	
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		const FGameplayTagContainer MergeTags = GetCombinedTags(FGameplayTag());
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergeTags);
		
		MeshComponent->SetSkeletalMesh(DesiredMesh, bReInitPose);
		
		if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			MeshComponent->SetPhysicsAsset(PhysicsAsset, bReInitPose);
		}
	}
}

FLinkCharacterPartHandle ULinkPawnComponent_CharacterParts::AddCharacterPart(const FLinkCharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

void ULinkPawnComponent_CharacterParts::RemoveCharacterPart(FLinkCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}
