// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkPlayerCameraManager.h"

ALinkPlayerCameraManager::ALinkPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultFOV = LINK_CAMERA_DEFAULT_FOV;
	ViewPitchMax = LINK_CAMERA_DEFAULT_PITCH_MAX;
	ViewPitchMin = LINK_CAMERA_DEFAULT_PITCH_MIN;
}
