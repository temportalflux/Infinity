#include "DiscordState.h"

#include "DiscordGameSDK.h"

void UDiscordState::initialize(int64_t clientId)
{
	discord::Core *pCore = nullptr;
	discord::Result result = discord::Core::Create(clientId, DiscordCreateFlags_Default, &pCore);
	if (pCore == nullptr)
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to instantiate Discord handshake"));
		return;
	}

	mpCore = MakeShareable(pCore);

	mpCore->SetLogHook(discord::LogLevel::Debug,
		[](discord::LogLevel level, const char* message)
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
	);

	mpCore->UserManager().OnCurrentUserUpdate.Connect(std::bind(
		&UDiscordState::onCurrentUserUpdate, this));
	
}

bool UDiscordState::isValid() const
{
	return mpCore.IsValid();
}

void UDiscordState::pollCallbacks()
{
	if (!mpCore.IsValid()) return;
	mpCore->RunCallbacks();
}

void UDiscordState::onCurrentUserUpdate()
{
	auto result = mpCore->UserManager().GetCurrentUser(&mCurrentUser);
	UE_LOG(LogDiscord, Log, TEXT("Updated current discord user to %s"), mCurrentUser.GetUsername());
	EventOnUpdateCurrentUser.Broadcast();
}
