// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LinkCharacterPartTypes.h"
#include "LinkCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "LinkPawnComponent_CharacterParts.generated.h"

class ULinkPawnComponent_CharacterParts;

// 인스턴스화 된 Character Part, 그 단위
USTRUCT()
struct FLinkAppliedCharacterPartEntry
{
	GENERATED_BODY()
	
public:
	// CharacterPart 에 대한 메타 데이터 (정의)
	UPROPERTY()
	FLinkCharacterPart Part;
	
	// LinkCharacterPartList 에서 할당 받은 Part Handle 값, 
	// LinkControllerCharacterPartEntry 의 Handle 값과 동일해야 한다
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
	
	// Character Part 용 ActorComponent
	// Character 가 바뀔 경우 그에 맞게 Part 도 변경되어야 하니까 관련 정보를 가지고 있음
	// UChildActorComponent 란? Actor 와 Actor 의 결합을 위해서 사용
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

// LinkPawnComponent_CharacterParts 에서 실질적으로 Character Parts 를 관리하는 구조체
USTRUCT(BlueprintType)
struct FLinkCharacterPartList
{
	GENERATED_BODY()
	
public:
	FLinkCharacterPartList()
		: OwnerComponent(nullptr)
	{}
	
	FLinkCharacterPartList(ULinkPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}
	
	// Member Methods
	bool SpawnActorForEntry(FLinkAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FLinkAppliedCharacterPartEntry& Entry);
	
	FGameplayTagContainer CollectCombinedTags() const;
	
	FLinkCharacterPartHandle AddEntry(FLinkCharacterPart NewPart);
	
	void RemoveEntry(FLinkCharacterPartHandle Handle);
	
	// Member Variables
	
	// 현재 인스턴스화된 Character Part
	UPROPERTY()
	TArray<FLinkAppliedCharacterPartEntry> Entries;
	
	// 인스턴스화 된 Character Parts 를 들고 있는 Owner, PawnComponent 를 가리킴
	UPROPERTY()
	TObjectPtr<ULinkPawnComponent_CharacterParts> OwnerComponent;
	
	// PartHandle 의 값을 할당 및 관리하는 변수
	int32 PartHandleCounter = 0;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class ULinkPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	ULinkPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	
	void BroadcastChanged();
	
	FLinkCharacterPartHandle AddCharacterPart(const FLinkCharacterPart& NewPart);
	void RemoveCharacterPart(FLinkCharacterPartHandle Handle);
	
	// 인스턴스화 된 Character Parts
	UPROPERTY()
	FLinkCharacterPartList CharacterPartList;
	
	// 애니메이션 적용을 위한 Meshes 와 연결고리
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FLinkAnimBodyStyleSelectionSet BodyMeshes;
};
