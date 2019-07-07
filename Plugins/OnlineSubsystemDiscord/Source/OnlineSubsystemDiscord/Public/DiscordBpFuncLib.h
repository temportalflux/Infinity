// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DiscordBpFuncLib.generated.h"

class UDiscordState;

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMDISCORD_API UDiscordBpFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = Discord)
		static UDiscordState* InitDiscord();
		
	UFUNCTION(BlueprintPure, Category = Discord)
		static UDiscordState* GetDiscord();
	
};
