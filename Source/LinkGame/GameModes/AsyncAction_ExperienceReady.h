// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

class AGameStateBase;
class ULinkExperienceDefinition;

UCLASS()
class LINKGAME_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);
	
	virtual void Activate() override;
	
	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const ULinkExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();
	
	// BlueprintAssignable 은 BP 에서 값을 할당할 수 있는 변수로 만듦
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;
	
	TWeakObjectPtr<UWorld> WorldPtr;
};
