#pragma once

#include "CoreMinimal.h"

#include "ItemStack.generated.h"

class UItem;

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UItem* Item;

};
