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

IMPLEMENT_PRIMARY_GAME_MODULE( FLinkGameModule, LinkGame, "LinkGame" );
