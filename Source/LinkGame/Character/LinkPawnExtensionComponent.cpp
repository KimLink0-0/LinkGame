// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkPawnExtensionComponent.h"

#include "LinkGameplayTags.h"
#include "LinkLogChannels.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LinkPawnData.h"

const FName ULinkPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

void ULinkPawnExtensionComponent::SetPawnData(const ULinkPawnData* InPawnData)
{
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	
	if (PawnData)
	{
		return;
	}
	
	// PawnData Update
	PawnData = InPawnData;
}

void ULinkPawnExtensionComponent::SetupPlayerInputComponent()
{
	// 강제 업데이트, InitState 상태 변환 시작
	// 왜 여기서 진행하나? Character 의 SetupPlayerInputComponent 가 실행되는 타이밍이 Possess 함수가 실행 된 뒤
	// 즉, PlayerController 를 사용할 준비가 된 시점
	CheckDefaultInitialization();
}

void ULinkPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// 나를 제외한 Feature 를 관찰 하는 중, InitState_DataAvailable 로 변경 된 Feature 가 있다면
	// CheckDefaultInitialization 을 실행, 전부 다 InitState_DataInitialized 로 변경될 수 있도록 업데이트 진행
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULinkPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);
	
	APawn* Pawn = GetPawn<APawn>();
	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();
	
	// Spawned
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}
	
	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!PawnData)
		{
			return false;
		}
		
		// SetPawnData 가 실행 될 시점엔 Controller 가 아직 준비되지 않음
		// 한 번은 여기서 멈추기 때문에 '다른 함수'에서 강제로 업데이트를 시켜줄 연결 고리가 필요
		// SetupPlayerInputComponent (Controller 와 PlayerState 가 준비 된) 시점에서 업데이트를 시킬 예정
		// Controller 는 ExtensionComponent 에서, PlayerState 는 FeatureComponent 에서 사용
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}
		
		return true;
	}
	
	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Actor 에 등록된 '모든' Feature 들이 DataAvailable 상태일 때, DataInitialized 로 넘어감 
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}
	
	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}
	
	// Out Case
	return false;
}

ULinkPawnExtensionComponent::ULinkPawnExtensionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ULinkPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();
	
	// 올바른 Actor 에 등록되어 있는지 확인
	if (!GetPawn<APawn>())
	{
		UE_LOG(LogLink, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
		return;
	}
	
	// 해당 함수를 사용하기 위해서는 IGameFrameworkInitStateInterface 를 상속해야 한다.
	// GameFrameworkComponentManager 에 등록된 InitState 를 사용하기 위해
	// RegisterInitStateFeature 함수를 통해서 (OwnerActor 와 Feature, ThisObject) 를 등록함
	RegisterInitStateFeature();
	
	// 디버깅을 위한 함수
	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}

void ULinkPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// FeatureName 에 NAME_None 을 넣으면, Actor 에 등록된 모든 Feature Component 의 InitState 상태를 관찰하겠다는 의미
	// 특정 FeatureName 을 넣으면 해당 Feature Component 의 InitState 상태를 관찰하겠다는 의미
	// RequiredState 에 FGameplayTag() 를 넣으면 모든 상태를 관찰하겠다는 의미
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);
	
	// TryToChangeInitState 의 진행 순서
	// 1. CanChangeInitState 로 상태 변환 가능성 유무를 판단 (CanChangeInitState 를 상속해서 조건을 우리가 커스텀)
	// 2. HandleChangeInitState 로 내부 상태 변경 (이 함수 또한 상속해서 커스텀)
	// 3. BindOnActorInitStateChanged 로 Bind 된 Delegate 를 조건에 맞게 호출
	// - 상태 변화에 따라 호출되는 함수는 OnActorInitStateChanged() 이다.
	ensure(TryToChangeInitState(FLinkGameplayTags::Get().InitState_Spawned));
	
	
	CheckDefaultInitialization();
}

void ULinkPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 앞서, OnRegister 안에 RegisterInitStateFeature 를 사용했으니 쌍으로 맞추기 위해 추가
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void ULinkPawnExtensionComponent::CheckDefaultInitialization()
{
	// this Object 를 제외한 모든 Feature 들을 강제로 업데이트 하는 함수
	// 내부적으로 있는 GetAllFeatureImplementers 코드를 확인! (this object 제외하고 선택)
	// 선택된 Feature 들은 CheckDefaultInitialization 을 통해 ContinueInitStateChain 을 활용하여 업데이트
	CheckDefaultInitializationForImplementers();
	
	
	// 상태 업데이트가 가능한 시점까지 업데이트
	// CheckDefaultInitializationForImplementers 함수의 내부에서 이 Feature 를 제외한 모든 Feature 들을 강제 업데이트
	// 어떤 함수를 통해서? CheckDefaultInitialization 을 통해서!
	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	
	ContinueInitStateChain(StateChain);
	
	// 결과적으로 모든 this object 를 포함한 모든 feature 들을 업데이트가 가능한 시점까지 강제로 업데이트가 된다.
}
