// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct FLinkGameplayTags
{
public:
	static const FLinkGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeTags(); 
	
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);
	
	// UGameFrameworkComponentManager 에서 사용하게 될 초기화 과정을 GameplayTag 로 등록
	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;
	
private:
	// static 변수 초기화 : .cpp 에서 필수 
	static FLinkGameplayTags GameplayTags;
};
