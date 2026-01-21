// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LinkCharacterPartTypes.generated.h"

USTRUCT(BlueprintType)
struct FLinkCharacterPartHandle
{
	GENERATED_BODY()
	
	void Reset()
	{
		PartHandle = INDEX_NONE;
	}
	
	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}
	
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FLinkCharacterPart
{
	GENERATED_BODY()
	
	// Part 로 생성된 Actor 의 Class 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;
	
	// Actor 를 붙이는데 필요한 Bone 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};