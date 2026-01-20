// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddInputConfig.h"

#include "EnhancedInputSubsystems.h"
#include "Character/LinkHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "PlayerMappableInputConfig.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddInputConfig)

void UGameFeatureAction_AddInputConfig::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(Context);
	if (!ensure(ActiveData.ExtensionRequestHandles.IsEmpty()) || !ensure(ActiveData.PawnsAddedTo.IsEmpty()))
	{
		Reset(ActiveData);
	}
	
	Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddInputConfig::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	
	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

void UGameFeatureAction_AddInputConfig::AddToWorld(const FWorldContext& WorldContext,
	const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	
	if (GameInstance && World && World->IsGameWorld())
	{
		// GameFrameworkComponentManager 를 이용해서, ExtensionHandler 를 추가하여 등록 
		// HandlePawnExtension 콜백 함수로 연결
		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddConfigDelegate = 
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandlePawnExtension, ChangeContext);
			
			// 등록된 콜백 함수의 핸들을 ActiveData 의 ExtensionRequestHandles 에 등록
			// 모든 Pawn 에 Extension 콜이 오면 AddConfigDelegate 를 실행 
			// 이벤트와 델리게이트의 차이 관련해서 학습 (28강)
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentMan->AddExtensionHandler(APawn::StaticClass(), AddConfigDelegate);
			ActiveData.ExtensionRequestHandles.Add(ExtensionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddInputConfig::HandlePawnExtension(AActor* Actor, FName EventName,
	FGameFeatureStateChangeContext ChangeContext)
{
	APawn* AsPawn = CastChecked<APawn>(Actor);
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	
	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == ULinkHeroComponent::NAME_BindInputsNow)
	{
		AddInputConfig(AsPawn, ActiveData);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved)
	{
		RemoveInputConfig(AsPawn, ActiveData);
	}
}

void UGameFeatureAction_AddInputConfig::AddInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			FModifyContextOptions Options = {};
			Options.bIgnoreAllPressedKeysUntilRelease = false;
		
			for (const FLinkMappableConfigPair& Pair : InputConfigs)
			{
				if (Pair.bShouldActivateAutomatically)
				{
					Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);	
				}
			}
			
			ActiveData.PawnsAddedTo.AddUnique(Pawn);
		}
	}
}

void UGameFeatureAction_AddInputConfig::RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			for (const FLinkMappableConfigPair& Pair : InputConfigs)
			{
				Subsystem->RemovePlayerMappableConfig(Pair.Config.LoadSynchronous());
			}
			
			ActiveData.PawnsAddedTo.Remove(Pawn);
		}
	}
}

void UGameFeatureAction_AddInputConfig::Reset(FPerContextData& ActiveData)
{
	ActiveData.ExtensionRequestHandles.Empty();
	
	while (!ActiveData.PawnsAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APawn> PawnPtr = ActiveData.PawnsAddedTo.Top();
		if (PawnPtr.IsValid())
		{
			RemoveInputConfig(PawnPtr.Get(), ActiveData);
		}
		else
		{
			// WeakObjectPtr 로 PawnsAddedTo 를 관리하고 있기 때문에, 가비지컬렉션이 되었다면, nullptr
			ActiveData.PawnsAddedTo.Pop();
		}
	}
}
