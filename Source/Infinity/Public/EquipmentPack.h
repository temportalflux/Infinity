#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "ItemStack.h"

#include "EquipmentPack.generated.h"

UCLASS()
class INFINITY_API UEquipmentPack : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FItemStack> Items;

};
