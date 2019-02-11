#pragma once

#include "CoreMinimal.h"

#include "DiceRoll.h"

#include "Damage.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Bludgeoning UMETA(DisplayName = "Bludgeoning"),
	Piercing UMETA(DisplayName = "Piercing"),
	Slashing UMETA(DisplayName = "Slashing"),
};

USTRUCT(BlueprintType)
struct FDamage
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDiceRoll Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDamageType Type;

};
