// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCharacter.h"
#include "Components/GameFrameworkComponentManager.h"

void AModularCharacter::PreInitializeComponents()
{
	// Components 초기화 전
	Super::PreInitializeComponents();
	
	// 게임 Feature 에서 AddComponent 등의 기능을 사용할 수 있게 됨
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::EndPlay(EndPlayReason);
}

void AModularCharacter::BeginPlay()
{
	// 게임 Feature 에서 AddComponent 등의 기능을 사용할 수 없게 됨
	// 순서가 바뀌면 간헐적으로 Crash 발생함
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::BeginPlay();
}
