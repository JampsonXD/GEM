#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"


class IGEMLoadingScreenModule : public IModuleInterface
{
public:
	/** Loads the module so it can be turned on **/
	static inline IGEMLoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IGEMLoadingScreenModule>("GEMLoadingScreen");
	}

	/** Starts the loading screen for in game loading (Not on startup) **/
	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) = 0;

	/** Stops the loading screen **/
	virtual void StopInGameLoadingScreen() = 0;
	
};
