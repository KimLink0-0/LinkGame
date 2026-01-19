// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LinkCameraMode.generated.h"

class ULinkCameraComponent;

struct FLinkCameraModeView
{
	FLinkCameraModeView();

	void Blend(const FLinkCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};


UENUM(BlueprintType)
enum class ELinkCameraModeBlendFunction : uint8
{
	Linear,
	
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

UCLASS(Abstract, NotBlueprintable)
class ULinkCameraMode : public UObject
{
	GENERATED_BODY()
public:
	ULinkCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/**
	 * member methods
	 */
	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	ULinkCameraComponent* GetLinkCameraComponent() const;
	AActor* GetTargetActor() const;
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;


	/**
	 * member variables
	 */
	
	FLinkCameraModeView View;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	float BlendAlpha;

	float BlendWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	/** Blend function */
	ELinkCameraModeBlendFunction BlendFunction;
};


UCLASS()
class ULinkCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	ULinkCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * member methods
	 */
	ULinkCameraMode* GetCameraModeInstance(TSubclassOf<ULinkCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<ULinkCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FLinkCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FLinkCameraModeView& OutCameraModeView) const;

	/**
	* member variables
	*/

	UPROPERTY()
	TArray<TObjectPtr<ULinkCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<ULinkCameraMode>> CameraModeStack;
};