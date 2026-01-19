// Fill out your copyright notice in the Description page of Project Settings.

#include "LinkPlayerCameraManager.h"

ALinkPlayerCameraManager::ALinkPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultFOV = LINK_CAMERA_DEFAULT_FOV;
	ViewPitchMin = LINK_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = LINK_CAMERA_DEFAULT_PITCH_MAX;
}
