// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "LinkGame/Input/LinkMappableConfigPair.h"
#include "GameFeatureAction_AddInputConfig.generated.h"

struct FComponentRequestHandle;
class APawn;

UCLASS()
class LINKGAME_API UGameFeatureAction_AddInputConfig : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:
	// UGameFeatureAction's Interface
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	
	// UGameFeatureAction_WorldActionBase's Interface
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	
private:
	// GameFeatureSubsystem 은 EngineSubsystem 이기에 
	// PIE 모드를 실행하고 꺼도 관련 데이터가 남아있을 가능성이 있고
	// 이를 방지하기 위해 Pawn 을 캐싱, 이를 이용해서 관련 데이터를 Reset 하는 로직 사용
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};
	
	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
	
	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	
	void Reset(FPerContextData& ActiveData);
	
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
	
	UPROPERTY(EditAnywhere)
	TArray<FLinkMappableConfigPair> InputConfigs;
};
