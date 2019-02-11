#pragma once

#include "CoreMinimal.h"
#include "DiceRoll.generated.h"

USTRUCT(BlueprintType)
struct FDiceRoll
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Size;

};
