#pragma once

#include "CoreMinimal.h"
#include "./discordsdk/discord.h"
#include "DiscordActivity.generated.h"

USTRUCT(BlueprintType)
struct FDiscordActivityAssets
{
	GENERATED_BODY()

public:

	// Keyname of an asset to display.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString imageLarge;

	// Hover text for the large image.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString textLarge;

	// Keyname of an asset to display.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString imageSmall;

	// Hover text for the small image.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString textSmall;

};

USTRUCT(BlueprintType)
struct FDiscordParty
{
	GENERATED_BODY()

public:

	// A unique identifier for this party.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FGuid id;

	// The max possible size of the party.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxSize;

	// The current size of the party.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int CurrentSize;

};

USTRUCT(BlueprintType)
struct FDiscordActivitySecrets
{
	GENERATED_BODY()

public:

	// A unique hash for the given match context.
	UPROPERTY()
		FGuid match;

	// A unique hash for chat invites and Ask to Join.
	UPROPERTY()
		FGuid join;

	// A unique hash for Spectate button.
	UPROPERTY()
		FGuid spectate;

};

USTRUCT(BlueprintType)
struct FDiscordActivity
{
	GENERATED_BODY()

public:

	// Current party status.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString state;

	// Current action. What the player is doing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString details;

	// Assets to display on the player's profile.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDiscordActivityAssets assets;

	// Assets to display on the player's profile.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDiscordParty party;

	// Assets to display on the player's profile.
	UPROPERTY()
		FDiscordActivitySecrets secrets;

	// Assets to display on the player's profile.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isMatch;

};

discord::Activity CreateDiscordActivity(FDiscordActivity const &activityBp);

discord::Activity DiscordActivityFromBpActivity(FDiscordActivity const &activityBp);

FDiscordActivity BlueprintActivityFromDiscordActivity(discord::Activity const &activityCore);
