#pragma once

#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "DiscordUser.generated.h"

namespace discord{ class User; };

USTRUCT(BlueprintType)
struct FDiscordUser
{
	GENERATED_BODY()

public:

	UPROPERTY()
		int64 id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString username;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int descriminator;

	UPROPERTY()
		FString avatarUrl;

	static FDiscordUser CreateFrom(discord::User const &user);

	FDiscordUser();

	FDiscordUser(int64 id) : id(id) {}

	/**
	 * Copy Constructor
	 */
	FDiscordUser(const FDiscordUser& src)
		: id(src.id)
		, username(src.username)
		, descriminator(src.descriminator)
		, avatarUrl(src.avatarUrl)
	{
	}

	void CopyFrom(discord::User const *const user);

	void Invalidate();

};
