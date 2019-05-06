#pragma once

#include "CoreMinimal.h"
#include "./discordsdk/discord.h"
#include "DiscordEnums.h"
#include "DiscordState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDiscordSimple);

UCLASS()
class UDiscordState : public UObject
{
	GENERATED_BODY()

	discord::Core *mpCore;
	discord::User mCurrentUser;

public:
	UDiscordState();
	~UDiscordState();

	UPROPERTY(BlueprintAssignable, Category = Events)
		FOnDiscordSimple EventOnUpdateCurrentUser;

	void initialize(int64_t clientId);

	bool isValid() const;

	DiscordResult pollCallbacks();

	void outputLog(discord::LogLevel level, const char* message);
	void onCurrentUserUpdate();

};
