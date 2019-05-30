// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SrcGameMode.generated.h"

USTRUCT(Blueprintable)
struct FCombatParticipant
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 initiative;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 idPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FGuid idRpgCharacter;

};

UCLASS(minimalapi)
class ASrcGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	ASrcGameMode();

	UFUNCTION(BlueprintCallable)
		TArray<FCombatParticipant> SortCombatParticipants(TArray<FCombatParticipant> participants);

};



