// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Item.h"

#include "MeshInfo.h"

#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS()
class INFINITY_API UItemObject : public UItem
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Render")
		FMeshInfo MeshInfo;
	
	
};
