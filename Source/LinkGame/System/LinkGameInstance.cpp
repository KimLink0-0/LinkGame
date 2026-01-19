// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkGameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LinkGame/LinkGameplayTags.h"

void ULinkGameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		const FLinkGameplayTags& GameplayTags = FLinkGameplayTags::Get();

		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void ULinkGameInstance::Shutdown()
{
	Super::Shutdown();
}
