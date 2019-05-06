// Fill out your copyright notice in the Description page of Project Settings.

#include "SrcGameInstance.h"

USrcGameInstance::USrcGameInstance()
{
	mpHttpModule = &FHttpModule::Get();
}

void USrcGameInstance::MakeRequest()
{
	TSharedRef<IHttpRequest> Request = mpHttpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &USrcGameInstance::OnRequestResponse);
	//This is the url on which to process the request
	Request->SetURL("https://discordapp.com/api/oauth2/authorize");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void USrcGameInstance::OnRequestResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{

}

