#include "DiscordState.h"

#include "DiscordGameSDK.h"

// START MACRO: Initialization check
#define INITIALIZED_GUARD(action) if (!IsInitialized()) \
{ \
	UE_LOG(LogDiscord, Warning, TEXT("Cannot " action " without initialization")); \
	return DiscordResult::NotRunning; \
}
// END MACRO

UDiscordState::UDiscordState()
	: UObject()
{
	mpCore = nullptr;
	EventOnUpdateCurrentUser.Clear();
	EventOnReceivedUserById.Clear();
}

UDiscordState::~UDiscordState()
{
	EventOnUpdateCurrentUser.Clear();
	EventOnReceivedUserById.Clear();
	if (mpCore != nullptr)
	{
		delete mpCore;
		mpCore = nullptr;
	}
}

DiscordResult UDiscordState::Initialize(int64 clientId)
{
	mLastResult = discord::Core::Create(clientId, DiscordCreateFlags_Default, &mpCore);
	
	UE_LOG(LogDiscord, Log, TEXT("Initialized discord core with id:%i, result: %i"), clientId, mLastResult);
	
	if (mpCore == nullptr)
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to instantiate Discord handshake"));
		mLastResult = discord::Result::NotRunning;
		return (DiscordResult)mLastResult;
	}

	//auto logFunc = std::bind(
	//	&UDiscordState::outputLog,
	//	this, std::placeholders::_1, std::placeholders::_2
	//);
	//mpCore->SetLogHook(discord::LogLevel::Error, logFunc);
	//mpCore->SetLogHook(discord::LogLevel::Warn, logFunc);
	//mpCore->SetLogHook(discord::LogLevel::Info, logFunc);
	//mpCore->SetLogHook(discord::LogLevel::Debug, logFunc);

	UE_LOG(LogDiscord, Log, TEXT("Attempting to connect with discord"));
	//mpCore->UserManager().OnCurrentUserUpdate.Connect(std::bind(
	//	&UDiscordState::onCurrentUserUpdate, this));
	mpCore->UserManager().OnCurrentUserUpdate.Connect([]() {
		UE_LOG(LogDiscord, Log, TEXT("User update via lambda"));
	});

	discord::Activity activity{};
	activity.SetDetails("Fruit Tarts");
	activity.SetState("Pop Snacks");
	activity.GetAssets().SetSmallImage("the");
	activity.GetAssets().SetSmallText("i mage");
	activity.GetAssets().SetLargeImage("the");
	activity.GetAssets().SetLargeText("u mage");
	activity.SetType(discord::ActivityType::Playing);
	mpCore->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
		UE_LOG(LogDiscord, Log, TEXT("%s updating activity!"),
			*FString((result == discord::Result::Ok) ? "Succeeded" : "Failed")
		);
	});
	
	return (DiscordResult)mLastResult;
}

bool UDiscordState::IsInitialized() const
{
	return mpCore != nullptr;
}

DiscordResult UDiscordState::PollCallbacks()
{
	INITIALIZED_GUARD("poll for discord updates")
	mLastResult = mpCore->RunCallbacks();
	return (DiscordResult)mLastResult;
}

void UDiscordState::outputLog(discord::LogLevel level, const char* message)
{
	switch (level)
	{
	case discord::LogLevel::Error:
		UE_LOG(LogDiscord, Error, TEXT("[DiscordSdk] %s"), message);
		break;
	case discord::LogLevel::Warn:
		UE_LOG(LogDiscord, Warning, TEXT("[DiscordSdk] %s"), message);
		break;
	case discord::LogLevel::Info:
		UE_LOG(LogDiscord, Log, TEXT("[DiscordSdk] %s"), message);
		break;
	case discord::LogLevel::Debug:
		UE_LOG(LogDiscord, Log, TEXT("[DiscordSdk] [DEBUG] %s"), message);
		break;
	}
}

DiscordResult UDiscordState::GetLastResult() const
{
	return (DiscordResult)mLastResult;
}

FDiscordUser UDiscordState::createUserFromDiscord(discord::User const &user) const
{
	return FDiscordUser {
			user.GetId(),
			FString(user.GetUsername()),
			FCString::Atoi(*FString(user.GetDiscriminator())),
			FString(user.GetAvatar()),
	};
}

FDiscordUser UDiscordState::GetCurrentUser(bool& valid) const
{
	valid = false;
	discord::User currentUser;
	auto result = mpCore->UserManager().GetCurrentUser(&currentUser);

	if (result != discord::Result::Ok)
	{
		return {};
	}

	valid = true;
	return createUserFromDiscord(currentUser);
}

void UDiscordState::onCurrentUserUpdate()
{
	bool userValid = false;
	auto user = this->GetCurrentUser(userValid);
	UE_LOG(LogDiscord, Log, TEXT("Updated current discord user to %s#%i"),
		*user.username, user.descriminator
	);
	if (userValid && this->EventOnUpdateCurrentUser.IsBound())
	{
		this->EventOnUpdateCurrentUser.Broadcast(user);
	}
}

DiscordResult UDiscordState::FetchUserById(int64 const id)
{
	INITIALIZED_GUARD("fetch user from id");
	mpCore->UserManager().GetUser(discord::UserId(id), std::bind(
		&UDiscordState::onReceivedUserFetch, this,
		std::placeholders::_1, std::placeholders::_2
	));
	return DiscordResult::Ok; // TODO: tell user that action is in callback
}

void UDiscordState::onReceivedUserFetch(discord::Result result, discord::User const &user)
{
	if (result == discord::Result::Ok && this->EventOnReceivedUserById.IsBound())
	{
		this->EventOnReceivedUserById.Broadcast(createUserFromDiscord(user));
	}
}
