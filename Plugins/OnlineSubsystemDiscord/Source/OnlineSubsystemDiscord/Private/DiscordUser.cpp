#include "DiscordUser.h"

#include "OSSDiscordTypes.h"
#include "OSSNames.h"
#include "./discordsdk/discord.h"

FDiscordUser FDiscordUser::CreateFrom(discord::User const &core)
{
	auto user = FDiscordUser();
	user.CopyFrom(&core);
	return user;
}

FDiscordUser::FDiscordUser()
	: id(0)
	, username("")
	, descriminator(0)
	, avatarUrl("")
{
}

void FDiscordUser::CopyFrom(discord::User const *const user)
{
	this->id = user->GetId();
	this->username = FString(user->GetUsername());
	this->descriminator = FCString::Atoi(*FString(user->GetDiscriminator()));
	this->avatarUrl = FString(user->GetAvatar());
}

void FDiscordUser::Invalidate()
{
	this->id = 0;
	this->username.Empty();
	this->descriminator = 0;
	this->avatarUrl.Empty();
}
