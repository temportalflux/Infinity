#pragma once

#include "CoreMinimal.h"
#include "./discordsdk/discord.h"
#include "DiscordState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDiscordSimple);

UCLASS()
class UDiscordState : public UObject
{
	GENERATED_BODY()

	TSharedPtr<discord::Core> mpCore;
	discord::User mCurrentUser;

public:

	FOnDiscordSimple EventOnUpdateCurrentUser;

	void initialize(int64_t clientId);

	bool isValid() const;

	void pollCallbacks();

	void onCurrentUserUpdate();

};
