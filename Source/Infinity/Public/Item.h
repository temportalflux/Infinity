// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "Price.h"
#include "GameplayTagContainer.h"

#include "Item.generated.h"

class UPaperSprite;

UCLASS()
class INFINITY_API UItem : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
		FGuid Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
		FPrice Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
		uint8 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
		FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Render")
		UPaperSprite* Sprite;

};
