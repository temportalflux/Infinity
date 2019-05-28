// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DiscordEnums.h"
#include "DiscordFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DISCORDGAMESDK_API UDiscordFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = Discord)
		static UDiscordState* CreateDiscordState();

	/*
	UFUNCTION(BlueprintPure, Category = Discord)
		static class UDiscordState* GetDiscordState(bool &initialized);

	UFUNCTION(BlueprintCallable, Category = Discord)
		static void DiscordInitialize(int64 clientID);

	UFUNCTION(BlueprintPure, Category = Discord)
		static bool DiscordHasBeenInitialized();

	UFUNCTION(BlueprintCallable, Category = Discord)
		static DiscordResult DiscordPollCallbacks();
	//*/
	
};
