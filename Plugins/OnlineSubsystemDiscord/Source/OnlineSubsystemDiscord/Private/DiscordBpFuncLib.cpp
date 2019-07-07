#include "DiscordBpFuncLib.h"
#include "OnlineSubsystemDiscordModule.h"
#include "./discordsdk/discord.h"
#include "DiscordState.h"

UDiscordState* UDiscordBpFuncLib::InitDiscord()
{
	auto discord = NewObject<UDiscordState>();
	discord->Init();
	return discord;
	//return FOnlineSubsystemDiscordModule::InitDiscord();
}

UDiscordState* UDiscordBpFuncLib::GetDiscord()
{
	return nullptr;// FOnlineSubsystemDiscordModule::GetDiscord();
	/*
	auto discordSubsystem = FOnlineSubsystemDiscordModule::GetSubsystem();
	if (!discordSubsystem.IsValid())
	{
		UE_LOG(LogDiscord, Error, TEXT("Cannot set discord activity, subsystem is invalid."));
		return NULL;
	}

	return discordSubsystem.Get()->GetBlueprintWrapper();
	/*
	auto discordCore = discordSubsystem.Get()->GetDiscord();
	if (discordCore == NULL)
	{
		UE_LOG(LogDiscord, Error, TEXT("Cannot set discord activity, discord sdk instance is invalid."));
		return;
	}
	//*/
}
