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
	
	EaseIn,			// 지수함수 그래프 모양 (기울기가 완만하다가 점점 가파르게)
	EaseOut,		// 로그함수 그래프 모양 (기울기가 가파르다가 점점 완만하게)
	EaseInOut,		// 위 둘을 합친 그래프 모양
	COUNT
};


UCLASS(Abstract, NotBlueprintable)
class LINKGAME_API ULinkCameraMode : public UObject
{
	GENERATED_BODY()
	
public:
	ULinkCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;
	AActor* GetTargetActor() const;
	ULinkCameraComponent* GetLinkCameraComponent() const;
	
	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTIme);
	
public:
	FLinkCameraModeView View;
	
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170"))
	float FieldOfView;
	
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;
	
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;
	
	// Blend 관련 변수
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;
	float BlendAlpha;
	float BlendWeight;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;
	
	ELinkCameraModeBlendFunction BlendFunction;
};

UCLASS()
class ULinkCameraModeStack : public UObject
{
	GENERATED_BODY()
	
public:
	ULinkCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// Member Function
	ULinkCameraMode* GetCameraModeInstance(TSubclassOf<ULinkCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<ULinkCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FLinkCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FLinkCameraModeView& OutCameraModeView);
	
	
public:
	// 생성된 CameraMode 를 관리 
	UPROPERTY()
	TArray<TObjectPtr<ULinkCameraMode>> CameraModeInstances;
	
	// Camera Matrix Blend 업데이트 진행 큐
	UPROPERTY()
	TArray<TObjectPtr<ULinkCameraMode>> CameraModeStack;
};
