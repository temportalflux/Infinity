// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActorHelperLib.generated.h"

/**
 * 
 */
UCLASS()
class UActorHelperLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	* Executes in Editor only. Puts the actor in the provided folder in the world outliner.
	*/
	UFUNCTION(BlueprintCallable, Category = "Actor")
		static void SetActorEditorFolder(AActor *actor, FName const folder);

	/**
	* Executes in Editor only. Puts the actor in the folder of the provided source-actor in the world outliner.
	*/
	UFUNCTION(BlueprintCallable, Category = "Actor")
		static void MatchActorEditorFolder(AActor *actor, AActor *sourceActor);
	
	/**
	* Renames the actor object, and if in editor, sets the actor label.
	*/
	UFUNCTION(BlueprintCallable, Category = "Actor")
		static void SetActorName(AActor *actor, FName const displayName);

};
