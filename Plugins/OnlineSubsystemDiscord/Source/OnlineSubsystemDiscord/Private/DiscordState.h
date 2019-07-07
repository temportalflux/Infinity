#pragma once

#include "CoreMinimal.h"

#include "./discordsdk/discord.h"
#include "DiscordEnums.h"
#include "DiscordUser.h"
#include "DiscordActivity.h"

#include "DiscordState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDiscordSimple);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiscordResult, DiscordResult, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiscordUser, FDiscordUser, user);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveImage, UTexture2D*, image);

UCLASS(Blueprintable)
class ONLINESUBSYSTEMDISCORD_API UDiscordState : public UObject
{
	GENERATED_BODY()

private:

	/** Discord App/Client ID for the running game */
	discord::ClientId mClientId;

	discord::Core* mpCore;

	/** If the Discord APIs been initialized */
	bool mbInitialized;

	discord::Result mLastResult;

	FDiscordActivity mCurrentActivity;

#pragma region Management

public:
	UDiscordState();
	~UDiscordState();

	bool IsInitialized() const;
	void Init();
	void Shutdown();

	FString GetAppId() const;

	UFUNCTION(BlueprintCallable)
		DiscordResult PollCallbacks();

#pragma endregion

#pragma region Binding Calls
private:
	std::function<void(discord::Result)> bindCallbackResult(
		void (UDiscordState::*memberFunc)(discord::Result))
	{
		return std::bind(memberFunc, this, std::placeholders::_1);
	}

	template <typename TData>
	std::function<void(discord::Result, TData)> bindCallbackResultData1(
		void (UDiscordState::*memberFunc)(discord::Result, TData))
	{
		return std::bind(memberFunc, this, std::placeholders::_1, std::placeholders::_2);
	}
#pragma endregion

#pragma region Passive Callbacks

	// ~~~~~~~~~~ OnCurrentUserUpdate ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	// OnCurrentUserUpdate: Callback
	UPROPERTY(BlueprintAssignable)
		FOnDiscordUser OnCurrentUserUpdate_Callback;
private:
	// OnCurrentUserUpdate: Response
	void OnCurrentUserUpdate_Response();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma endregion

#pragma region Synchronous Actions

public:
	// GetCurrentUser: Call
	UFUNCTION(BlueprintCallable)
		FDiscordUser GetCurrentUser(bool& valid) const;

#pragma endregion

#pragma region User

	// ~~~~~~~~~~ FetchUserById ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	// FetchUserById: Call
	DiscordResult FetchUserById(int64 const id);
private:
	// FetchUserById: Response
	void FetchUserById_Response(discord::Result result, discord::User const &user);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma endregion

#pragma region Activity

	// ~~~~~~~~~~ UpdateActivity ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	// UpdateActivity: Callback
	UPROPERTY(BlueprintAssignable)
		FOnDiscordResult ActvivityUpdate_Callback;

	// UpdateActivity: Call
	UFUNCTION(BlueprintCallable)
		void UpdateActivity(FDiscordActivity activity);
private:
	// UpdateActivity: Response
	void UpdateActivity_Response(discord::Result result);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// ~~~~~~~~~~ ClearActivity ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	// ClearActivity: Callback
	UPROPERTY(BlueprintAssignable)
		FOnDiscordResult ClearActvivity_Callback;

	// ClearActivity: Call
	UFUNCTION(BlueprintCallable)
		void ClearActivity();
private:
	// ClearActivity: Response
	void ClearActivity_Response(discord::Result result);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma endregion

#pragma region UE Sessions

public:

	UFUNCTION(BlueprintCallable)
		bool CreateDiscordSession(APlayerController* PlayerController, FString MapName);

	UFUNCTION(BlueprintCallable)
		void JoinDiscordSession(APlayerController* PlayerController, FString HostAddress);
	
#pragma endregion


};
