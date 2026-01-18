// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkCameraMode_ThirdPerson.h"

#include "Curves/CurveVector.h"

ULinkCameraMode_ThirdPerson::ULinkCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

PRAGMA_DISABLE_OPTIMIZATION
void ULinkCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	// 기본적인 틀은 기존과 동일
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();
	
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);
	
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
	
	// 기본값에 커브(벡터) 값을 얼마를 넣을 진 모르겠지만, 해당 값을 더해준다.
	// 카메라가 있을 위치를 커브로 조절할 수 있음
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
PRAGMA_ENABLE_OPTIMIZATION
