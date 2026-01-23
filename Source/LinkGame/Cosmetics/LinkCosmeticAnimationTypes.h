// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LinkCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FLinkAnimLayerSelectionEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Layer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer RequiredTags;
};


USTRUCT(BlueprintType)
struct FLinkAnimLayerSelectionSet
{
	GENERATED_BODY()
	
public:
	TSubclassOf<UAnimInstance> SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLinkAnimLayerSelectionEntry> LayerRules;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> DefaultLayer;
	
};

USTRUCT(BlueprintType)
struct FLinkAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()
	
public:
	// AnimLayer 를 적용할 대상 SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;
	
	// Cosmetic Tag, Mesh 의 종류에 맞는 Tag
	// 아래 MeshRules 에서 사용할 태그, 인간형, 동물형 등
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FLinkAnimBodyStyleSelectionSet
{
	GENERATED_BODY()
	
public:
	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;
	
	// AnimLayer 를 적용할 SkeletalMesh, Animation-Mesh 간 규칙
	// 해당 Actor 가 어떤 모양의 SkeletalMesh 를 장착해야 하는지 관리하는 룰, ex) 인간형, 동물형 등 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLinkAnimBodyStyleSelectionEntry> MeshRules;
	
	// 기본으로 적용할 SkeletalMesh
	// 위 규칙에 맞는 대상이 없을 때 장착하는 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;
	
	// Physics Asset 은 하나로 동일
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};
