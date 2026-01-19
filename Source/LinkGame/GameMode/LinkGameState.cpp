// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkGameState.h"
#include "LinkExperienceManagerComponent.h"

ALinkGameState::ALinkGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<ULinkExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
