#include "OnlineSubsystemDiscordModule.h"

//#include "OnlineSubsystemModule.h"
//#include "OnlineSubsystem.h"
//#include "OSSNames.h"
//#include "OnlineSubsystemDiscord.h"
//#include "SharedPointer.h"

#include "DiscordState.h"

DEFINE_LOG_CATEGORY(LogDiscord);

IMPLEMENT_MODULE(FOnlineSubsystemDiscordModule, OnlineSubsystemDiscord);

#define LOCTEXT_NAMESPACE "FOnlineSubsystemDiscordModule"

/**
 * Class responsible for creating instance(s) of the subsystem
 */
/*
class FOnlineSubsystemDiscordFactory : public IOnlineFactory
{

private:

	// Single instantiation of the subsystem interface
	static FOnlineSubsystemDiscordPtr mpsSingleton;

	virtual void DestroySubsystem()
	{
		if (mpsSingleton.IsValid())
		{
			mpsSingleton->Shutdown();
			mpsSingleton = NULL;
		}
	}

public:

	FOnlineSubsystemDiscordFactory() {}
	virtual ~FOnlineSubsystemDiscordFactory()
	{
		DestroySubsystem();
	}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName)
	{
		UE_LOG(LogDiscord, Log, TEXT("Creating discord subsystem %s"), *InstanceName.ToString());
		if (!mpsSingleton.IsValid())
		{
			mpsSingleton = MakeShared<FOnlineSubsystemDiscord, ESPMode::ThreadSafe>(InstanceName);
			if (mpsSingleton->IsEnabled())
			{
				if (!mpsSingleton->Init())
				{
					UE_LOG(LogDiscord, Warning, TEXT("Discord SDK failed to initialize!"));
					DestroySubsystem();
				}
			}
			else
			{
				UE_CLOG(IsRunningDedicatedServer() || IsRunningGame(), LogDiscord, Warning, TEXT("Discord SDK disabled!"));
				DestroySubsystem();
			}

			return mpsSingleton;
		}

		UE_LOG(LogDiscord, Warning, TEXT("Can't create more than one instance of the Discord online subsystem!"));
		return NULL;
	}

	FOnlineSubsystemDiscordPtr GetSingleton() const
	{
		return mpsSingleton;
	}

};
//*/

//FOnlineSubsystemDiscordPtr FOnlineSubsystemDiscordFactory::mpsSingleton = NULL;

void FOnlineSubsystemDiscordModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Create and register our singleton factory with the main online subsystem for easy access
	//mpOSSDiscordFactory = new FOnlineSubsystemDiscordFactory();

	//FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	//OSS.RegisterPlatformService(DISCORD_SUBSYSTEM, mpOSSDiscordFactory);

}

void FOnlineSubsystemDiscordModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	//FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	//OSS.UnregisterPlatformService(DISCORD_SUBSYSTEM);

	//delete mpOSSDiscordFactory;
	//mpOSSDiscordFactory = NULL;
}

/*
FOnlineSubsystemDiscordPtr FOnlineSubsystemDiscordModule::GetSubsystem()
{
	if (FOnlineSubsystemDiscordModule::IsAvailable())
		return FOnlineSubsystemDiscordModule::Get().mpOSSDiscordFactory->GetSingleton();
	return NULL;
}
//*/

UDiscordState* FOnlineSubsystemDiscordModule::InitDiscord()
{
	if (FOnlineSubsystemDiscordModule::IsAvailable())
	{
		auto discord = NewObject<UDiscordState>();
		discord->Init();
		FOnlineSubsystemDiscordModule::Get().mpDiscord = discord;
		return discord;
	}
	return NULL;
}

UDiscordState* FOnlineSubsystemDiscordModule::GetDiscord()
{
	if (FOnlineSubsystemDiscordModule::IsAvailable())
		return FOnlineSubsystemDiscordModule::Get().mpDiscord;
	return NULL;
}

#undef LOCTEXT_NAMESPACE