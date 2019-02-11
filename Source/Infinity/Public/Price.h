#pragma once

#include "CoreMinimal.h"

#include "Price.generated.h"

UENUM(BlueprintType)
enum class ECurrency : uint8
{
	Copper UMETA(DisplayName = "Copper"),
	Silver UMETA(DisplayName = "Silver"),
	Electrum UMETA(DisplayName = "Electrum"),
	Gold UMETA(DisplayName = "Gold"),
	Platinum UMETA(DisplayName = "Platinum"),
};

USTRUCT(BlueprintType)
struct INFINITY_API FPrice
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECurrency Currency;

};
