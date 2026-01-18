// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LinkPawnData.generated.h"

class ULinkCameraMode;

UCLASS()
class LINKGAME_API ULinkPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// 초기화 생성자
	ULinkPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Link|Pawn")
	TSubclassOf<APawn> PawnClass;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Link|Pawn")
	TSubclassOf<ULinkCameraMode> DefaultCameraMode;
};
