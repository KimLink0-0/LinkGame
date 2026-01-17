// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "LinkPawnExtensionComponent.generated.h"

class ULinkPawnData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LINKGAME_API ULinkPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	ULinkPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// FeatureName 정의
	static const FName NAME_ActorFeatureName;
	
	virtual FName GetFeatureName() const override final { return NAME_ActorFeatureName; }
	
	// Member Function
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const ULinkPawnData* InPawnData);
	static ULinkPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return ( Actor ? Actor->FindComponentByClass<ULinkPawnExtensionComponent>() : nullptr); }
	void SetupPlayerInputComponent();
	
	
	// IGameFrameworkInitStateInterface
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	
	virtual void CheckDefaultInitialization() final;
	
	
	// Lifecycle 
	void OnRegister() final;
	
	void BeginPlay() final;
	
	void EndPlay(const EEndPlayReason::Type EndPlayReason) final;
	
public:
	// 생성한 PawnData 를 캐싱
	UPROPERTY(EditInstanceOnly, Category = "Link|Pawn")
	TObjectPtr<const ULinkPawnData> PawnData;
	
};
