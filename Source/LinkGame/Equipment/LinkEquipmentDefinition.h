// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LinkEquipmentDefinition.generated.h"

class ULinkEquipmentInstance;

USTRUCT()
struct FLinkEquipmentActorToSpawn
{
	GENERATED_BODY()
	
public:
	// Spawn 할 대상 Actor 클래스
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;
	
	// 어느 Bone Socket 에 붙일 지 
	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;
	
	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};

UCLASS(Blueprintable)
class LINKGAME_API ULinkEquipmentDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	ULinkEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// 메타 데이터를 이용해서 어떤 Instance 를 Spawn 할 지 결정하기 위한 클래스
	// 여기에서 Instance 는 관리하기 위한 주체?
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<ULinkEquipmentInstance> InstanceType;
	
	
	// 해당 장착 아이템을 사용하면, 어떤 Actor 가 Spawn 되는 지에 관한 정보
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FLinkEquipmentActorToSpawn> ActorsToSpawn;
};
