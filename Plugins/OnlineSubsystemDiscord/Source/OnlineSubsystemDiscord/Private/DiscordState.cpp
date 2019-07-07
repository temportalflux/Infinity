#include "DiscordState.h"

#include "ConfigCacheIni.h"
#include "HAL/FileManager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Serialization/BufferArchive.h"
#include "Misc/Paths.h"
#include "Misc/CommandLine.h"

#include "Online.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"

#include "OnlineSubsystemDiscordModule.h"

#pragma region Macros

// ~~~~~~~~~~ Initialization check ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define INITIALIZED_GUARD(action, ret) if (!IsInitialized()) \
{ \
	UE_LOG(LogDiscord, Warning, TEXT("Cannot " action " without initialization")); \
	ret; \
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma endregion

#pragma region Configuration

/* Specify this define in your Target.cs for your project
 *
 * This helps the DiscordGameSDK find your project on shipping builds.
 */
#ifndef UE4_PROJECT_DISCORDSHIPPINGID
#define UE4_PROJECT_DISCORDSHIPPINGID 0
#endif

 /** Filename containing the appid during development */
#define DISCORD_CLIENT_ID_FILENAME TEXT("steam_appid.txt")

inline FString GetDiscordClientIdFilename()
{
	return FString::Printf(TEXT("%s%s"), FPlatformProcess::BaseDir(), DISCORD_CLIENT_ID_FILENAME);
}

#if !UE_BUILD_SHIPPING && !UE_BUILD_SHIPPING_WITH_EDITOR

class FScopeSandboxContext
{
private:
	/** Previous state of sandbox enable */
	bool bSandboxWasEnabled;

	FScopeSandboxContext() {}

public:
	FScopeSandboxContext(bool bSandboxEnabled)
	{
		bSandboxWasEnabled = IFileManager::Get().IsSandboxEnabled();
		IFileManager::Get().SetSandboxEnabled(bSandboxEnabled);
	}

	~FScopeSandboxContext()
	{
		IFileManager::Get().SetSandboxEnabled(bSandboxWasEnabled);
	}
};

/**
 * Write out the steam app id to the steam_appid.txt file before initializing the API
 * @param SteamAppId id assigned to the application by Steam
 */
static void WriteDiscordClientIdToDisk(int64 ClientId)
{
	if (ClientId > 0)
	{
		// Turn off sandbox temporarily to make sure file is where it's always expected
		FScopeSandboxContext ScopedSandbox(false);

		// Access the physical file writer directly so that we still write next to the executable in CotF builds.
		FString filename = GetDiscordClientIdFilename();
		IFileHandle* Handle = IPlatformFile::GetPlatformPhysical().OpenWrite(*filename, false, false);
		if (!Handle)
		{
			UE_LOG(LogDiscord, Fatal, TEXT("Failed to create file: %s"), *filename);
		}
		else
		{
			FString id = FString::Printf(TEXT("%i"), ClientId);

			FBufferArchive Archive;
			Archive.Serialize((void*)TCHAR_TO_ANSI(*id), id.Len());

			Handle->Write(Archive.GetData(), Archive.Num());
			delete Handle;
			Handle = nullptr;
		}
	}
}

/**
 * Deletes the app id file from disk
 */
static void DeleteDiscordClientIdFromDisk()
{
	const FString filename = GetDiscordClientIdFilename();
	// Turn off sandbox temporarily to make sure file is where it's always expected
	FScopeSandboxContext ScopedSandbox(false);
	if (FPaths::FileExists(*filename))
	{
		bool bSuccessfullyDeleted = IFileManager::Get().Delete(*filename);
	}
}

#endif // !UE_BUILD_SHIPPING && !UE_BUILD_SHIPPING_WITH_EDITOR

/**
 * Configure various dev options before initializing Steam
 *
 * @param RequireRelaunch enforce the Steam client running precondition
 * @param RelaunchAppId appid to launch when the Steam client is loaded
 */
void ConfigurePluginInitDevOptions(int64& ClientId)
{
#if !UE_BUILD_SHIPPING && !UE_BUILD_SHIPPING_WITH_EDITOR
	// Write out the steam_appid.txt file before launching
	FConfigFile* File = GConfig->Find(GEngineIni, 0);
	if (!File)
	{
		UE_LOG(LogDiscord, Warning, TEXT("Missing DefaultEngine.ini config file."));
	}
	else if (!File->GetInt64(TEXT("OnlineSubsystemDiscord"), TEXT("DiscordDevClientId"), ClientId))
	{
		UE_LOG(LogDiscord, Warning, TEXT("Missing DiscordDevClientId key in OnlineSubsystemDiscord of DefaultEngine.ini"));
	}
	else
	{
		UE_LOG(LogDiscord, Log, TEXT("Found DiscordDevClientId %llu in OnlineSubsystemDiscord of DefaultEngine.ini"), ClientId);
		WriteDiscordClientIdToDisk(ClientId);
	}
#else
	// Always check against the Steam client when shipping
	ClientId = UE4_PROJECT_DISCORDSHIPPINGID;
#endif
}

#pragma endregion

#pragma region Management

UDiscordState::UDiscordState()
	: UObject()
{
	mpCore = nullptr;

	mCurrentActivity = FDiscordActivity();

	this->ActvivityUpdate_Callback.Clear();
	this->ClearActvivity_Callback.Clear();
	this->OnCurrentUserUpdate_Callback.Clear();
}

UDiscordState::~UDiscordState()
{
	this->ActvivityUpdate_Callback.Clear();
	this->ClearActvivity_Callback.Clear();
	this->OnCurrentUserUpdate_Callback.Clear();

	mCurrentActivity = FDiscordActivity();

	mpCore = nullptr;
}

bool InitializeDiscord(discord::ClientId const clientId, discord::Core *&discord, bool &initialized)
{
	UE_LOG(LogDiscord, Log, TEXT("Attempting to handshake with Discord using client id %llu"), clientId);
	discord::Result result = discord::Core::Create(clientId, DiscordCreateFlags_Default, &discord);
	initialized = result == discord::Result::Ok && discord != nullptr;
	if (!initialized)
	{
		UE_LOG(LogDiscord, Error, TEXT("Could not initialize Discord handshake: result = %llu"), int(result));
	}
	else
	{
		UE_LOG(LogDiscord, Log, TEXT("Successful initialized Discord handshake"));
	}
	return initialized;
}

bool UDiscordState::IsInitialized() const
{
	return mpCore != nullptr;
}

void UDiscordState::Init()
{
	int64 clientId = 0;
	ConfigurePluginInitDevOptions(clientId);

	const bool bIsServer = IsRunningDedicatedServer();
	bool bAttemptServerInit = true;

	// Don't initialize the Discord Client API if we are launching as a server
	//bool bClientInitSuccess = !bIsServer ? this->InitClient(clientId) : true;

	// Initialize the Steam Server API if this is a dedicated server or
	//  the Client API was successfully initialized
	//bool bServerInitSuccess = bClientInitSuccess ? (!bAttemptServerInit || this->InitServer()) : false;

	UE_LOG(LogDiscord, Log, TEXT("Initializing OnlineSubsystemDiscord with application id %llu"), clientId);

	mClientId = discord::ClientId(clientId);
	InitializeDiscord(mClientId, mpCore, mbInitialized);

	if (!mbInitialized)
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to initialize discord core"));
		Shutdown();
		return;
	}

	this->mpCore->UserManager().OnCurrentUserUpdate.Connect(std::bind(&UDiscordState::OnCurrentUserUpdate_Response, this));
}

void UDiscordState::Shutdown()
{
#if !UE_BUILD_SHIPPING && !UE_BUILD_SHIPPING_WITH_EDITOR
	DeleteDiscordClientIdFromDisk();
#endif // !UE_BUILD_SHIPPING && !UE_BUILD_SHIPPING_WITH_EDITOR
}

FString UDiscordState::GetAppId() const
{
	return FString::Printf(TEXT("%i"), mClientId);
}

DiscordResult UDiscordState::PollCallbacks()
{
	INITIALIZED_GUARD("poll for discord updates", return DiscordResult::NotRunning);
	mLastResult = mpCore->RunCallbacks();
	if (mLastResult != discord::Result::Ok)
	{
		UE_LOG(LogDiscord, Warning, TEXT("Poll callbacks returned non-ok result: %i"), mLastResult);
	}
	return (DiscordResult)mLastResult;
}

#pragma endregion

#pragma region Passive Callbacks

void UDiscordState::OnCurrentUserUpdate_Response()
{
	bool userValid = false;
	auto user = this->GetCurrentUser(userValid);
	UE_LOG(LogDiscord, Log, TEXT("Updated current discord user to %s#%i"),
		*user.username, user.descriminator
	);
	if (userValid && this->OnCurrentUserUpdate_Callback.IsBound())
	{
		this->OnCurrentUserUpdate_Callback.Broadcast(user);
	}
}

#pragma endregion

#pragma region Synchronous Actions

FDiscordUser UDiscordState::GetCurrentUser(bool& valid) const
{
	INITIALIZED_GUARD("get the current user", return {});
	valid = false;
	discord::User currentUser;
	auto result = mpCore->UserManager().GetCurrentUser(&currentUser);

	if (result != discord::Result::Ok)
	{
		return {};
	}

	valid = true;
	return FDiscordUser::CreateFrom(currentUser);
}

#pragma endregion

#pragma region User

// ~~~~~~~~~~ FetchUserById ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DiscordResult UDiscordState::FetchUserById(int64 const id)
{
	INITIALIZED_GUARD("fetch user from id", return DiscordResult::NotRunning);
	mpCore->UserManager().GetUser(discord::UserId(id), this->bindCallbackResultData1(&UDiscordState::FetchUserById_Response));
	return DiscordResult::Ok; // TODO: tell user that action is in callback
}

void UDiscordState::FetchUserById_Response(discord::Result result, discord::User const &user)
{
	/*
	if (result == discord::Result::Ok && this->EventOnReceivedUserById.IsBound())
	{
		this->EventOnReceivedUserById.Broadcast(FDiscordUser::CreateFrom(user));
	}
	//*/
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma endregion

#pragma region Activity

// ~~~~~~~~~~ UpdateActivity ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void UDiscordState::UpdateActivity(FDiscordActivity activity)
{
	INITIALIZED_GUARD("update presence activity", );
	mCurrentActivity = activity;
	this->mpCore->ActivityManager().UpdateActivity(CreateDiscordActivity(activity), bindCallbackResult(&UDiscordState::UpdateActivity_Response));
}

void UDiscordState::UpdateActivity_Response(discord::Result result)
{
	if (this->ActvivityUpdate_Callback.IsBound())
	{
		this->ActvivityUpdate_Callback.Broadcast((DiscordResult)result);
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~ ClearActivity ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void UDiscordState::ClearActivity()
{
	INITIALIZED_GUARD("clear presence activity", );
	mCurrentActivity = FDiscordActivity();
	this->mpCore->ActivityManager().ClearActivity(bindCallbackResult(&UDiscordState::UpdateActivity_Response));
}

void UDiscordState::ClearActivity_Response(discord::Result result)
{
	if (this->ClearActvivity_Callback.IsBound())
	{
		this->ClearActvivity_Callback.Broadcast((DiscordResult)result);
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma endregion

#pragma region Sessions

bool UDiscordState::CreateDiscordSession(APlayerController* PlayerController, FString MapName)
{
	if (PlayerController == NULL) { return false; }
	if (PlayerController->GetWorld() == NULL) { return false; }
	auto travelArgs = FString::Printf(TEXT("%s?listen"), *MapName);
	UE_LOG(LogDiscord, Log, TEXT("Creating discord session host. Server travel to \"%s\"."), *travelArgs);
	return PlayerController->GetWorld()->ServerTravel(travelArgs);
}

void UDiscordState::JoinDiscordSession(APlayerController* PlayerController, FString HostAddress)
{
	if (PlayerController == NULL) { return; }
	UE_LOG(LogDiscord, Log, TEXT("Creating discord session client. Client travel to \"%s\"."), *HostAddress);
	PlayerController->ClientTravel(*HostAddress, ETravelType::TRAVEL_Absolute);
}

#pragma endregion

