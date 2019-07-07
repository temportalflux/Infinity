#include "DiscordActivity.h"
#include "OnlineSubsystemDiscordModule.h"

inline char const * stringToChars(FString str)
{
	return TCHAR_TO_ANSI(*str);
}

inline char const * ConvertGuidToDiscordId(FGuid const &guid)
{
	return stringToChars(guid.ToString());
}

discord::Activity CreateDiscordActivity(FDiscordActivity const &activityBp)
{
	auto partyGuid = FGuid::NewGuid();
	auto matchGuid = FGuid::NewGuid();
	auto joinGuid = FGuid::NewGuid();

	UE_LOG(LogDiscord, Warning, TEXT("Creating discord activity. Party:%s Match:%s Join:%s"),
		*partyGuid.ToString(), *matchGuid.ToString(), *joinGuid.ToString()
	);

	auto activityCore = discord::Activity();

	activityCore.SetState(stringToChars(activityBp.state));
	activityCore.SetDetails(stringToChars(activityBp.details));

	auto& assets = activityCore.GetAssets();
	assets.SetLargeImage(stringToChars(activityBp.assets.imageLarge));
	assets.SetLargeText(stringToChars(activityBp.assets.textLarge));
	assets.SetSmallImage(stringToChars(activityBp.assets.imageSmall));
	assets.SetSmallText(stringToChars(activityBp.assets.textSmall));

	auto& party = activityCore.GetParty();
	party.SetId(ConvertGuidToDiscordId(partyGuid));
	auto& partySize = party.GetSize();
	partySize.SetMaxSize(activityBp.party.MaxSize);
	partySize.SetCurrentSize(1);

	auto& secrets = activityCore.GetSecrets();
	secrets.SetMatch(ConvertGuidToDiscordId(matchGuid));
	secrets.SetJoin(ConvertGuidToDiscordId(joinGuid));
	//secrets.SetSpectate(ConvertGuidToDiscordId(spectateGuid));

	activityCore.SetInstance(false);

	return activityCore;
}

discord::Activity DiscordActivityFromBpActivity(FDiscordActivity const &activityBp)
{
	auto activityCore = discord::Activity();

	activityCore.SetState(stringToChars(activityBp.state));
	activityCore.SetDetails(stringToChars(activityBp.details));

	auto& assets = activityCore.GetAssets();
	assets.SetLargeImage(stringToChars(activityBp.assets.imageLarge));
	assets.SetLargeText(stringToChars(activityBp.assets.textLarge));
	assets.SetSmallImage(stringToChars(activityBp.assets.imageSmall));
	assets.SetSmallText(stringToChars(activityBp.assets.textSmall));

	auto& party = activityCore.GetParty();
	party.SetId(ConvertGuidToDiscordId(activityBp.party.id));
	auto& partySize = party.GetSize();
	partySize.SetMaxSize(activityBp.party.MaxSize);
	partySize.SetCurrentSize(activityBp.party.CurrentSize);

	auto& secrets = activityCore.GetSecrets();
	secrets.SetMatch(ConvertGuidToDiscordId(activityBp.secrets.match));
	secrets.SetJoin(ConvertGuidToDiscordId(activityBp.secrets.join));
	secrets.SetSpectate(ConvertGuidToDiscordId(activityBp.secrets.spectate));

	activityCore.SetInstance(activityBp.isMatch);

	return activityCore;
}

FDiscordActivity BlueprintActivityFromDiscordActivity(discord::Activity const &activityCore)
{
	auto activityBp = FDiscordActivity();

	activityBp.state = FString(activityCore.GetState());
	activityBp.details = FString(activityCore.GetDetails());

	auto assets = activityCore.GetAssets();
	activityBp.assets.imageLarge = FString(assets.GetLargeImage());
	activityBp.assets.textLarge = FString(assets.GetLargeText());
	activityBp.assets.imageSmall = FString(assets.GetSmallImage());
	activityBp.assets.textSmall = FString(assets.GetSmallText());

	auto party = activityCore.GetParty();
	FGuid::Parse(FString(party.GetId()), activityBp.party.id);
	auto partySize = party.GetSize();
	activityBp.party.MaxSize = partySize.GetMaxSize();
	activityBp.party.CurrentSize = partySize.GetCurrentSize();

	auto secrets = activityCore.GetSecrets();
	FGuid::Parse(FString(secrets.GetMatch()), activityBp.secrets.match);
	FGuid::Parse(FString(secrets.GetJoin()), activityBp.secrets.join);
	FGuid::Parse(FString(secrets.GetSpectate()), activityBp.secrets.spectate);

	activityBp.isMatch = activityCore.GetInstance();

	return activityBp;
}
