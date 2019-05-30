// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SrcPlayerState.generated.h"

UCLASS()
class INFINITY_API ASrcPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	/**
	* The ID of the user on Discord.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, replicated)
		int64 idDiscord;

	/**
	* The ID of the RPG character data block.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, replicated)
		FGuid idRpgCharacter;
	
	UFUNCTION(BlueprintCallable)
		int32 GetNetworkId() const;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

};
