#pragma once

#include "CoreMinimal.h"

// Parent
#include "Engine/GameInstance.h"

// HTTP lib
#include "Runtime/Online/HTTP/Public/Http.h"

#include "SrcGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFINITY_API USrcGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:

	FHttpModule* mpHttpModule;
	
public:
	USrcGameInstance();
	
	UFUNCTION(BlueprintCallable)
		void MakeRequest();

	void OnRequestResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

};
