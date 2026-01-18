// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkPlayerController.h"

#include "Camera/LinkPlayerCameraManager.h"

ALinkPlayerController::ALinkPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ALinkPlayerCameraManager::StaticClass();
}
