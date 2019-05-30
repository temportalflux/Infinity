// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SrcGameMode.h"
#include "UObject/ConstructorHelpers.h"

ASrcGameMode::ASrcGameMode()
{
	FMath::RandInit((int32)(FDateTime::Now().GetTicks() % INT_MAX));
}

TArray<FCombatParticipant> ASrcGameMode::SortCombatParticipants(TArray<FCombatParticipant> participants)
{
	participants.Sort([](const FCombatParticipant& a, const FCombatParticipant& b)
	{
		if (a.initiative > b.initiative) return true;
		else if (a.initiative == b.initiative) return FMath::Rand() % 2 == 0;
		else return false;
	});
	return participants;
}
