// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkHeroComponent.h"
#include "LinkPawnData.h"
#include "LinkPawnExtensionComponent.h"
#include "PlayerMappableInputConfig.h"
#include "LinkGame/Input/LinkMappableConfigPair.h"
#include "LinkGame/Input/LinkInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LinkGame/LinkGameplayTags.h"
#include "LinkGame/LinkLogChannels.h"
#include "LinkGame/Camera/LinkCameraComponent.h"
#include "LinkGame/Player/LinkPlayerController.h"
#include "LinkGame/Player/LinkPlayerState.h"

const FName ULinkHeroComponent::NAME_ActorFeatureName("Hero");

ULinkHeroComponent::ULinkHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void ULinkHeroComponent::OnRegister()
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
}

void ULinkHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(ULinkPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	ensure(TryToChangeInitState(FLinkGameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void ULinkHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void ULinkHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();

	if (Params.FeatureName == ULinkPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULinkHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,FGameplayTag DesiredState) const
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
		return LinkPS && Manager->HasFeatureReachedInitState(Pawn, ULinkPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void ULinkHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,FGameplayTag DesiredState)
{
	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();

	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALinkPlayerState* LinkPS = GetPlayerState<ALinkPlayerState>();
		if (!ensure(Pawn && LinkPS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const ULinkPawnData* PawnData = nullptr;
		if (ULinkPawnExtensionComponent* PawnExtComp = ULinkPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULinkPawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			if (ULinkCameraComponent* CameraComponent = ULinkCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}

		if (ALinkPlayerController* LinkPC = GetController<ALinkPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void ULinkHeroComponent::CheckDefaultInitialization()
{
	const FLinkGameplayTags& InitTags = FLinkGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

TSubclassOf<ULinkCameraMode> ULinkHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (ULinkPawnExtensionComponent* PawnExtComp = ULinkPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULinkPawnData* PawnData = PawnExtComp->GetPawnData<ULinkPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void ULinkHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const ULinkPawnExtensionComponent* PawnExtComp = ULinkPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULinkPawnData* PawnData = PawnExtComp->GetPawnData<ULinkPawnData>())
		{
			if (const ULinkInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FLinkGameplayTags& GameplayTags = FLinkGameplayTags::Get();

				for (const FLinkMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);	
					}
				}

				ULinkInputComponent* LinkIC = CastChecked<ULinkInputComponent>(PlayerInputComponent);
				{
					LinkIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					LinkIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}
}

void ULinkHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);

			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f) 
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ULinkHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}
