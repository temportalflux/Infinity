#include "DiscordState.h"

#include "DiscordGameSDK.h"

UDiscordState::UDiscordState()
	: UObject()
{
	mpCore = nullptr;
}

UDiscordState::~UDiscordState()
{
	if (mpCore != nullptr)
	{
		delete mpCore;
		mpCore = nullptr;
	}
}

void UDiscordState::initialize(int64_t clientId)
{
	discord::Result result = discord::Core::Create(clientId,
		DiscordCreateFlags_Default, &mpCore);
	
	UE_LOG(LogDiscord, Log, TEXT("Initialized discord core, result: %i"), result);
	
	if (mpCore == nullptr)
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to instantiate Discord handshake"));
		return;
	}

	auto logFunc = std::bind(
		&UDiscordState::outputLog,
		this, std::placeholders::_1, std::placeholders::_2
	);
	mpCore->SetLogHook(discord::LogLevel::Error, logFunc);
	mpCore->SetLogHook(discord::LogLevel::Warn, logFunc);
	mpCore->SetLogHook(discord::LogLevel::Info, logFunc);
	mpCore->SetLogHook(discord::LogLevel::Debug, logFunc);

	UE_LOG(LogDiscord, Log, TEXT("Attempting to connect with discord"));
	mpCore->UserManager().OnCurrentUserUpdate.Connect(std::bind(
		&UDiscordState::onCurrentUserUpdate, this));
	
}

bool UDiscordState::isValid() const
{
	return mpCore != nullptr;
}

DiscordResult UDiscordState::pollCallbacks()
{
	if (!isValid())
	{
		UE_LOG(LogDiscord, Warning, TEXT("Cannot poll for discord updates without initialization"));
		return DiscordResult::NotRunning;
	}

	auto result = mpCore->RunCallbacks();
	if (result != discord::Result::Ok)
		UE_LOG(LogDiscord, Warning, TEXT("Encountered non-ok discord result %i"), result);
	return (DiscordResult)result;
}

void UDiscordState::outputLog(discord::LogLevel level, const char* message)
{
	switch (level)
	{
	case discord::LogLevel::Error:
		UE_LOG(LogDiscord, Error, TEXT("%s"), message);
		break;
	case discord::LogLevel::Warn:
		UE_LOG(LogDiscord, Warning, TEXT("%s"), message);
		break;
	case discord::LogLevel::Info:
		UE_LOG(LogDiscord, Log, TEXT("%s"), message);
		break;
	case discord::LogLevel::Debug:
		UE_LOG(LogDiscord, Log, TEXT("[DEBUG] %s"), message);
		break;
	}
}

void UDiscordState::onCurrentUserUpdate()
{
	auto result = mpCore->UserManager().GetCurrentUser(&mCurrentUser);
	UE_LOG(LogDiscord, Log, TEXT("Updated current discord user to %s"), mCurrentUser.GetUsername());
	EventOnUpdateCurrentUser.Broadcast();
}
