// Fill out your copyright notice in the Description page of Project Settings.

#include "DiscordFunctionLibrary.h"

#include "DiscordGameSDK.h"

UDiscordState* UDiscordFunctionLibrary::GetDiscordState(bool &valid)
{
	valid = false;
	if (!FDiscordGameSDKModule::IsAvailable())
	{
		UE_LOG(LogDiscord, Warning, TEXT("Discord module not available"));
		return nullptr;
	}
	auto state = FDiscordGameSDKModule::Get().getState();
	valid = state.IsValid();
	return state.Pin().Get();
}

void UDiscordFunctionLibrary::DiscordInitialize(int64 clientId)
{
	if (!FDiscordGameSDKModule::IsAvailable())
	{
		UE_LOG(LogDiscord, Warning, TEXT("Discord module not available"));
		return;
	}
	FDiscordGameSDKModule::Get().getState().Pin()->initialize(clientId);
}

bool UDiscordFunctionLibrary::DiscordHasBeenInitialized()
{
	return FDiscordGameSDKModule::IsAvailable()
		&& FDiscordGameSDKModule::Get().getState().Pin()->isValid();
}

DiscordResult UDiscordFunctionLibrary::DiscordPollCallbacks()
{
	if (!FDiscordGameSDKModule::IsAvailable())
	{
		UE_LOG(LogDiscord, Warning, TEXT("Discord module not available"));
		return DiscordResult::NotRunning;
	}
	return FDiscordGameSDKModule::Get().getState().Pin()->pollCallbacks();
}

