// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "DiscordState.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDiscord, Log, All);

class FDiscordGameSDKModule : public IModuleInterface
{

private:
	UDiscordState mDiscord;

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FDiscordGameSDKModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FDiscordGameSDKModule>("DiscordGameSDK");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("DiscordGameSDK");
	}

	UDiscordState& getState()
	{
		return mDiscord;
	}
	
};
