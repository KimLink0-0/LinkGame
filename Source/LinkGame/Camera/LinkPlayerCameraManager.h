// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "LinkPlayerCameraManager.generated.h"

#define LINK_CAMERA_DEFAULT_FOV (80.0f)
#define LINK_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define LINK_CAMERA_DEFAULT_PITCH_MAX (89.0f)

UCLASS()
class LINKGAME_API ALinkPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ALinkPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
