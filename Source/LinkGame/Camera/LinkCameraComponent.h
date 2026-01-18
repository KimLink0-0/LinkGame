// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LinkCameraMode.h"
#include "Camera/CameraComponent.h"
#include "LinkCameraComponent.generated.h"

class ULinkCameraModeStack;

template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<ULinkCameraMode>, FLinkCameraModeDelegate);

UCLASS()
class LINKGAME_API ULinkCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	ULinkCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	static ULinkCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULinkCameraComponent>() : nullptr); }
	
	AActor* GetTargetActor() const { return GetOwner(); };
	
	// 
	virtual void OnRegister() final;
	
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	
	void UpdateCameraModes();
	
	UPROPERTY()
	TObjectPtr<ULinkCameraModeStack> CameraModeStack;
	
	FLinkCameraModeDelegate DetermineCameraModeDelegate;
};
