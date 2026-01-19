// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkCameraComponent.h"

#include "LinkCameraMode.h"

ULinkCameraComponent::ULinkCameraComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CameraModeStack(nullptr)
{
	
}

void ULinkCameraComponent::OnRegister()
{
	Super::OnRegister();

	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<ULinkCameraModeStack>(this);
	}
}

void ULinkCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();

	FLinkCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

	FieldOfView = CameraModeView.FieldOfView;

	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void ULinkCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf<ULinkCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}