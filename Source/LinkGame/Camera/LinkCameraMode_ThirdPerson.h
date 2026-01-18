// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/LinkCameraMode.h"
#include "LinkCameraMode_ThirdPerson.generated.h"

class UCurveVector;

UCLASS(Abstract, Blueprintable)
class LINKGAME_API ULinkCameraMode_ThirdPerson : public ULinkCameraMode
{
	GENERATED_BODY()
	
public:
	ULinkCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void UpdateView(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Third person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
