// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction_WorldActionBase.generated.h"

class FDelegateHandle;
class UGameInstance;
struct FGameFeatureActivatingContext;
struct FGameFeatureDeactivatingContext;
struct FWorldContext;

UCLASS(Abstract)
class LINKGAME_API UGameFeatureAction_WorldActionBase : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	// GameFeatureAction's Interface
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	
	
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGameFeatureAction_WorldActionBase::AddToWorld, );
};
