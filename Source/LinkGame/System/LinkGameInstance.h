// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LinkGameInstance.generated.h"


UCLASS()
class LINKGAME_API ULinkGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	virtual void Shutdown() override;
};
