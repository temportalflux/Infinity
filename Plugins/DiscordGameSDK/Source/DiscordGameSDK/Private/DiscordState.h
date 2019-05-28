#pragma once

#include "CoreMinimal.h"
#include "./discordsdk/discord.h"
#include "DiscordEnums.h"
#include "DiscordUser.h"
#include "DiscordState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDiscordSimple);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiscordUser, FDiscordUser, user);

UCLASS(Blueprintable)
class UDiscordState : public UObject
{
	GENERATED_BODY()

	discord::Core *mpCore;
	discord::Result mLastResult;

	// Events ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:

	UPROPERTY(BlueprintAssignable, Category = Events)
		FOnDiscordUser EventTmpOnUpdateCurrentUser;

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

	UFUNCTION(BlueprintPure, Category = User)
		FDiscordUser GetCurrentUser(bool& valid);

private:

	void onCurrentUserUpdate();

};
