// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkCameraMode.h"

#include "LinkCameraComponent.h"
#include "LinkPlayerCameraManager.h"

FLinkCameraModeView::FLinkCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(LINK_CAMERA_DEFAULT_FOV)
{
}

void FLinkCameraModeView::Blend(const FLinkCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.f)
	{
		return;
	}
	else if (OtherWeight >= 1.f)
	{
		// Other 의 Weight 가 1.f 이상이면 너무 오래된 데이터이기에 보간하지 않고 덮어 씌운다?
		// 가중치가 1.f 이 넘어가는 값은 사용하지 않는다
		*this = Other;
		return;
	}
	
	// 각각의 View 변수들을 보간
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);
	
	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);
	
	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);
	
	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

ULinkCameraMode::ULinkCameraMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FieldOfView = LINK_CAMERA_DEFAULT_FOV;
	ViewPitchMax = LINK_CAMERA_DEFAULT_PITCH_MAX;
	ViewPitchMin = LINK_CAMERA_DEFAULT_PITCH_MIN;
	
	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;
	
	BlendFunction = ELinkCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

void ULinkCameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	
	UpdateBlending(DeltaTime);
}

void ULinkCameraMode::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();
	
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);
	
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void ULinkCameraMode::UpdateBlending(float DeltaTIme)
{
	if (BlendTime > 0.f)
	{
		// DeltaTime 을 활용하여 진행 정도를 누적
		BlendAlpha += (DeltaTIme / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}
	
	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case ELinkCameraModeBlendFunction::Linear:
		{
			BlendWeight = BlendAlpha;
			break;	
		}
	case ELinkCameraModeBlendFunction::EaseIn:
		{
			BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
			break;	
		}
		
	case ELinkCameraModeBlendFunction::EaseOut:
		{
			BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);	
			break;
		}
	case ELinkCameraModeBlendFunction::EaseInOut:
		{
			BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
			break;
		}
	default:
		{
			checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
			break;
		}
		
	}
}

FVector ULinkCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check (TargetActor);
	
	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetPawnViewLocation();
	}
	
	return TargetActor->GetActorLocation();
}

FRotator ULinkCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);
	
	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// Pawn 의 Controller 의 Rotation 을 반환
		return TargetPawn->GetViewRotation();
	}
	
	return TargetActor->GetActorRotation();
}

AActor* ULinkCameraMode::GetTargetActor() const
{
	const ULinkCameraComponent* CameraComponent = GetLinkCameraComponent();
	return CameraComponent->GetTargetActor();
}

ULinkCameraComponent* ULinkCameraMode::GetLinkCameraComponent() const
{
	// 생성할 때 GetOuter 설정 가능, CameraMode 를 생성 할 때, CameraModeStack 에서 (카메라모드스택의) GetOuter 를 넣음
	// 그래서 CameraMode 의 Outer 는 CameraModeComponent 가 된다.
	return CastChecked<ULinkCameraComponent>(GetOuter());
}

ULinkCameraModeStack::ULinkCameraModeStack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

ULinkCameraMode* ULinkCameraModeStack::GetCameraModeInstance(TSubclassOf<ULinkCameraMode>& CameraModeClass)
{
	check(CameraModeClass);
	
	// 이미 생성되어 있는지 확인
	for (ULinkCameraMode* CameraMode : CameraModeInstances)
	{
		// 만약 이미 생성되어 있고, 그 클래스가 요청된 카메라 모드 클래스와 같을 경우 이미 만들어진 것을 반환
		// 예 ) FPS, TPS 모드가 이미 생성되어 있고, FPS 모드가 요청 되었을 경우 FPS 모드가 이미 존재하니 이걸 반환
		// 이렇게 만들 경우, CameraModeInstances 에 클래스 타입별 하나의 CameraMode 만이 생기게 된다
		if (CameraMode != nullptr && CameraMode->GetClass() == CameraModeClass)
		{
			return CameraMode;
		}
	}
	
	// 생성되어 있지 않다면, NewObject 로 생성해서 CameraModeInstances 에 저장 후 반환
	// CameraMode 는 많지 않지만 (캐싱 해도 부담이 적음), 자주 쓰일 수 있으니 캐싱해서 사용 
	// 캐싱 내용을 관리하는 로직도 필요할 것 같음
	ULinkCameraMode* NewCameraMode = NewObject<ULinkCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check (NewCameraMode);
	CameraModeInstances.Add(NewCameraMode);
	
	return NewCameraMode;
}

void ULinkCameraModeStack::PushCameraMode(TSubclassOf<ULinkCameraMode>& CameraModeClass)
{
	// 해당 로직은 CameraMode 를 추가, 갱신 작업과 0 번과 마지막 인덱스의 BlendWeight 를 계산 해주는 작업을 시행
	// 최신 CameraMode 가 들어올 때
	// 1. Stack 을 순회하여 기존에 있는 CameraMode 인지 찾아봄
	// 2 - 1. 기존에 있을 경우 Stack 에서 삭제, 계산된 ExistingStackContribution 로 BlendWeight 입력
	// 2 - 2. 기존에 없을 경우 BlendWeight 를 0.0f 을 입력
	// 3. GameMode 를 Stack 의 0 번 Index 에 추가
	// 4. Stack 의 마지막 Index 의 BlendWeight 를 1.0f 으로 변경
	
	if (!CameraModeClass)
	{
		return;
	}
	
	ULinkCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check (CameraMode);
	
	// 특이한 점
	// 현재 사용중인 최신 값이 0 Stack, 원래 스택은 최후미에 들어가게 되어있음.
	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		return;
	}
	
	int32 ExistingStackIndex = INDEX_NONE;
	// Contribution 은 BlendWeight 값을 찾기 위해 1.0f 으로 초기 설정
	float ExistingStackContribution = 1.0f;
	
	for (int32 StackIndex = 0; StackIndex < StackSize; StackIndex++)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;	
		}
		else
		{
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}
	
	// 앞의 로직에 따라 새로 들어온 CameraMode 가 기존에 있던 모드라면, 지워준다
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		// 새로운 CameraMode 가 들어오면 해당 CameraMode 의 Contribution 은 0.0f 에서 시작 
		ExistingStackContribution = 0.0f;
	}
	
	// BlendTime : Blend 를 얼마나 진행할 것인가? 
	// BlendTime = 0.f 일 경우, Blend 를 하지 않겠다는 의미. 그래서 bShouldBlend = false 가 된다. 
	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;
	
	// 해당 로직상 Stack 의 최신 값을 제일 앞에 Insert 로 삽입
	CameraModeStack.Insert(CameraMode, 0);
	
	// Stack 안의 CameraMode 들이 잘 섞이기 위해서 BlendWeight [0,1] 범위를 가짐
	// 그래서 마지막에 있는 Stack 의 BlendWeight 를 1.0f 으로 수정
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void ULinkCameraModeStack::EvaluateStack(float DeltaTime, FLinkCameraModeView& OutCameraModeView)
{
	// Top -> Bottom [0 -> Num] 까지 순차적으로 Stack 에 있는 CameraMode 업데이트
	UpdateStack(DeltaTime);
	
	// Bottom -> Top 까지 CameraModeStack 에 대한 Blending 진행
	BlendStack(OutCameraModeView);
}

void ULinkCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}
	
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	// CameraModeStack 을 순회
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		ULinkCameraMode* CameraMode = CameraModeStack[StackIndex];
		check (CameraMode);
		
		// UpdateCameraMode
		CameraMode->UpdateCameraMode(DeltaTime);
		
		// 만약 BlendWeight 가 1.0f 을 넘어갈 경우, 해당 Index 이후 CameraMode 를 제거하기 위한 Local Variables 수정
		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}
	
	// BlendWeight 1.0f 이상인 CameraMode 제거
	if (RemoveCount > 0)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void ULinkCameraModeStack::BlendStack(FLinkCameraModeView& OutCameraModeView)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}
	
	const ULinkCameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check (CameraMode);
	
	OutCameraModeView = CameraMode->View;
	
	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check (CameraMode);
		
		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}
