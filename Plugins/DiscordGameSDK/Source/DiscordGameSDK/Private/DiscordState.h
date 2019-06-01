#pragma once

#include "CoreMinimal.h"
#include "./discordsdk/discord.h"
#include "DiscordEnums.h"
#include "DiscordUser.h"

// For Image decoding

#include "DiscordState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDiscordSimple);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiscordUser, FDiscordUser, user);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveImage, FSlateBrush, image);

UCLASS(Blueprintable)
class UDiscordState : public UObject
{
	GENERATED_BODY()

	discord::Core *mpCore;
	discord::Result mLastResult;

	// Events ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:

	UPROPERTY(BlueprintAssignable, Category = "User")
		FOnDiscordUser EventOnUpdateCurrentUser;

	UPROPERTY(BlueprintAssignable, Category = "User")
		FOnDiscordUser EventOnReceivedUserById;

	UPROPERTY(BlueprintAssignable, Category = "User|Avatar")
		FOnReceiveImage EventOnReceivedUserAvatar;

	// Management ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	UDiscordState();
	~UDiscordState();

	UFUNCTION(BlueprintCallable)
		DiscordResult Initialize(int64 clientId);

	UFUNCTION(BlueprintPure)
		bool IsInitialized() const;

	UFUNCTION(BlueprintCallable)
		DiscordResult PollCallbacks();

	UFUNCTION(BlueprintPure)
		DiscordResult GetLastResult() const;

private:

	void outputLog(discord::LogLevel level, const char* message);
	
	// User ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:

	UFUNCTION(BlueprintPure, Category = "User")
		FDiscordUser GetCurrentUser(bool& valid) const;

	UFUNCTION(BlueprintCallable, Category = "User")
		DiscordResult FetchUserById(int64 const id);

	UFUNCTION(BlueprintCallable, Category = "User|Avatar")
		DiscordResult FetchUserAvatar(int64 const id, int32 const size);

private:

	template <typename TData>
	std::function<void(discord::Result, TData)> bindDiscordCallback(
		void (UDiscordState::*memberFunc)(discord::Result, TData))
	{
		return std::bind(memberFunc, this, std::placeholders::_1, std::placeholders::_2);
	}

	FDiscordUser createUserFromDiscord(discord::User const &user) const;

	void onCurrentUserUpdate();
	
	void onReceivedUserById(discord::Result result, discord::User const &user);

	void onReceivedUserAvatar(discord::Result res, discord::ImageHandle handle);

};
