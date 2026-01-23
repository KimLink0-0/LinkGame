// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkPawnExtensionComponent.h"

#include "AbilitySystem/LinkAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LinkGame/LinkGameplayTags.h"
#include "LinkGame/LinkLogChannels.h"

const FName ULinkPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

ULinkPawnExtensionComponent::ULinkPawnExtensionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}


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

	PawnData = InPawnData;
}

void ULinkPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void ULinkPawnExtensionComponent::InitializeAbilitySystem(ULinkAbilitySystemComponent* InAbilitySystemComponent,
	AActor* InOwnerActor)
{
	check (InAbilitySystemComponent && InOwnerActor);
	
	if (AbilitySystemComponent == InAbilitySystemComponent)
	{
		return;
	}
	
	// 시작
	{
		// 언리얼에서 자주 쓰는 패턴, 해제 후 다시 등록
		if (AbilitySystemComponent)
		{
			UninitializeAbilitySystem();
		}
	
		APawn* Pawn = GetPawnChecked<APawn>();
		AActor* ExistingAvatar = InAbilitySystemComponent->GetAvatarActor();
		check(!ExistingAvatar)
	
		// AbilitySystemComponent 를 캐싱하고, Owner 와 Avatar 설정
		AbilitySystemComponent = InAbilitySystemComponent;
		AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);
	}
}

void ULinkPawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	AbilitySystemComponent = nullptr;
}

void ULinkPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogLink, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	RegisterInitStateFeature();

	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}

void ULinkPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	ensure(TryToChangeInitState(FLinkGameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void ULinkPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void ULinkPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULinkPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState , FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();

	// InitState_Spawned
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
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void ULinkPawnExtensionComponent::CheckDefaultInitialization()
{
	CheckDefaultInitializationForImplementers();

	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();

	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	ContinueInitStateChain(StateChain);
}
