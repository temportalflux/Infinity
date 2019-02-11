#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
	Simple UMETA(DisplayName = "Simple"),
	Martial UMETA(DisplayName = "Martial"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
};
