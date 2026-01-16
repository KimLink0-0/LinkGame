// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkGameplayTags.h"
#include "GameplayTagsManager.h"

FLinkGameplayTags FLinkGameplayTags::GameplayTags;

void FLinkGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
}

void FLinkGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native)") + FString(TagComment)));
}

void FLinkGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initially spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for this Actor/Componenet");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this Actor/Component");
	AddTag(InitState_GameplayReady, "InitState.GameplayReady", "4: The Actor/Component is fully ready for active gameplay");
}
