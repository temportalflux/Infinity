// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ItemObject.h"

#include "Enums/WeaponEnums.h"
#include "Damage.h"

#include "ItemWeapon.generated.h"

/**
 * 
 */
UCLASS()
class INFINITY_API UItemWeapon : public UItemObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		EWeaponCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		EWeaponType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FDamage Damage;
	
};
