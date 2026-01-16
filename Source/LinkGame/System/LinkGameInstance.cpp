// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkGameInstance.h"

#include "LinkGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

void ULinkGameInstance::Init()
{
	Super::Init();
	
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	ensure(ComponentManager);
	
	if (ComponentManager)
	{
		const FLinkGameplayTags& GameplayTags = FLinkGameplayTags::Get();
		
		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_GameplayReady);
	}
}

void ULinkGameInstance::Shutdown()
{
	Super::Shutdown();
}
