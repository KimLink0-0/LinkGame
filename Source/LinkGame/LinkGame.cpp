// Copyright Epic Games, Inc. All Rights Reserved.

#include "LinkGame.h"
#include "Modules/ModuleManager.h"

class FLinkGameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FLinkGameModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
}

void FLinkGameModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

// PRIMARY_GAME_MODULE 은 엔진 내에 단 하나만 존재할 것
IMPLEMENT_PRIMARY_GAME_MODULE(FLinkGameModule, LinkGame, "LinkGame");
