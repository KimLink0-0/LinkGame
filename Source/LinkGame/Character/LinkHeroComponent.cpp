// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkHeroComponent.h"

#include "LinkGameplayTags.h"
#include "LinkLogChannels.h"
#include "LinkPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Player/LinkPlayerState.h"
#include "LinkPawnData.h"

const FName ULinkHeroComponent::NAME_ActorFeatureName("Hero");

ULinkHeroComponent::ULinkHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ULinkHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// Params : 관찰 대상이 여기서는 PawnExtensionComponent 이다. 
	if (Params.FeatureName != ULinkPawnExtensionComponent::NAME_ActorFeatureName)
	{
		const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULinkHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);
	
	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ALinkPlayerState* LinkPS = GetPlayerState<ALinkPlayerState>();
	
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
		if (!LinkPS)
		{
			return false;
		}
		
		return true;
	}
	
	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent 의 InitTags 를 체크
		// 필요하면 PawnExtensionComponent 에 이 부분을 비교 체크
		return LinkPS && Manager->HasFeatureReachedInitState(Pawn, ULinkPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataAvailable);
	}
	
	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}
	
	// Out Case
	return false;
}

void ULinkHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	// 이하 직접 핸들링할 로직을 작성
	
	
	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();
	
	// DataAvailable -> DataInitialized 
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALinkPlayerState* LinkPS = GetPlayerState<ALinkPlayerState>();
		if (!ensure(Pawn && LinkPS))
		{
			return;
		}
		
		// Input 과 Camera 에 대한 로직을 핸들링 
		
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const ULinkPawnData* PawnData = nullptr;
		if (ULinkPawnExtensionComponent* PawnExtComp = ULinkPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULinkPawnData>();
		}
	}
}

void ULinkHeroComponent::CheckDefaultInitialization()
{
	// CheckDefaultInitializationForImplementers(); 함수가 여기엔 없는 이유, 얘는 다른 Component 를 관리하는 ManagerComponent 가 아니기 때문 
	
	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	
	ContinueInitStateChain(StateChain);
}

void ULinkHeroComponent::OnRegister()
{
	Super::OnRegister();
	
	if (!GetPawn<APawn>())
	{
		UE_LOG(LogLink, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
	}
	
	RegisterInitStateFeature();
}

void ULinkHeroComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// PawnExtensionComponent 에 대해서 OnActorInitStateChanged() 를 관찰하도록 
	BindOnActorInitStateChanged(ULinkPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);
	
	ensure(TryToChangeInitState(FLinkGameplayTags::Get().InitState_Spawned));
	
	CheckDefaultInitialization();
}

void ULinkHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}
