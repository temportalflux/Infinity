#pragma once

#include "CoreMinimal.h"
#include "DiscordUser.generated.h"

USTRUCT(Blueprintable)
struct FDiscordUser
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int64 id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString username;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int descriminator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString avatarUrl;

};
