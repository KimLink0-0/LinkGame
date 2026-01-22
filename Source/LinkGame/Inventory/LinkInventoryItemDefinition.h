// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LinkInventoryItemDefinition.generated.h"

// 변화되어야 하는 값들을 에디터 상에서 추가하고 싶을 때 사용하는 키워드 DefaultToInstance, EditInlineNew, Instanced 관련 검색

UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class ULinkInventoryItemFragment : public UObject
{
	GENERATED_BODY()
};

UCLASS()
class LINKGAME_API ULinkInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	ULinkInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;
	
	// 해당 속성이 CDO 에 노출되기 원하기 떄문에 Instanced 키워드를 붙여야 한다?
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<ULinkInventoryItemFragment>> Fragments;
};
