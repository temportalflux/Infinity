// Fill out your copyright notice in the Description page of Project Settings.

#include "DiscordFunctionLibrary.h"

#include "DiscordGameSDK.h"

UDiscordState* UDiscordFunctionLibrary::GetDiscordState(bool &valid)
{
	valid = false;
	if (!FDiscordGameSDKModule::IsAvailable())
		return nullptr;
	auto* state = &FDiscordGameSDKModule::Get().getState();
	valid = state != nullptr;
	return state;
}

void UDiscordFunctionLibrary::DiscordInitialize(int64 clientId)
{
	if (!FDiscordGameSDKModule::IsAvailable())
		return;
	FDiscordGameSDKModule::Get().getState().initialize(clientId);
}

bool UDiscordFunctionLibrary::DiscordHasBeenInitialized()
{
	return FDiscordGameSDKModule::IsAvailable()
		&& FDiscordGameSDKModule::Get().getState().isValid();
}

void UDiscordFunctionLibrary::DiscordPollCallbacks()
{
	if (!FDiscordGameSDKModule::IsAvailable())
		return;
	FDiscordGameSDKModule::Get().getState().pollCallbacks();
}

