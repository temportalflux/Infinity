#include "DiscordState.h"

#include "DiscordGameSDK.h"

// For image decoding
#include "IImageWrapperModule.h"
#include "SlateApplication.h"
#include "SlateDynamicImageBrush.h"

// START MACRO: Initialization check
#define INITIALIZED_GUARD(action) if (!IsInitialized()) \
{ \
	UE_LOG(LogDiscord, Warning, TEXT("Cannot " action " without initialization")); \
	return DiscordResult::NotRunning; \
}
// END MACRO

UDiscordState::UDiscordState()
	: UObject()
{
	mpCore = nullptr;
	EventOnUpdateCurrentUser.Clear();
	EventOnReceivedUserById.Clear();
	EventOnReceivedUserAvatar.Clear();
}

UDiscordState::~UDiscordState()
{
	EventOnUpdateCurrentUser.Clear();
	EventOnReceivedUserById.Clear();
	EventOnReceivedUserAvatar.Clear();
	if (mpCore != nullptr)
	{
		delete mpCore;
		mpCore = nullptr;
	}
}

DiscordResult UDiscordState::Initialize(int64 clientId)
{
	UE_LOG(LogDiscord, Warning, TEXT("%i -> %i"), clientId, int64(574661303034183690));
	mLastResult = discord::Core::Create(discord::ClientId(574661303034183690), DiscordCreateFlags_Default, &mpCore);
	
	UE_LOG(LogDiscord, Log, TEXT("Initialized discord core with id:%i, result: %i"), clientId, mLastResult);
	
	if (mpCore == nullptr)
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to instantiate Discord handshake"));
		mLastResult = discord::Result::NotRunning;
		return (DiscordResult)mLastResult;
	}

	//auto logFunc = std::bind(
	//	&UDiscordState::outputLog,
	//	this, std::placeholders::_1, std::placeholders::_2
	//);
	//mpCore->SetLogHook(discord::LogLevel::Error, logFunc);
	//mpCore->SetLogHook(discord::LogLevel::Warn, logFunc);
	//mpCore->SetLogHook(discord::LogLevel::Info, logFunc);
	//mpCore->SetLogHook(discord::LogLevel::Debug, logFunc);

	UE_LOG(LogDiscord, Log, TEXT("Attempting to connect with discord"));
	mpCore->UserManager().OnCurrentUserUpdate.Connect(std::bind(
		&UDiscordState::onCurrentUserUpdate, this));

	discord::Activity activity{};
	activity.SetDetails("Fruit Tarts");
	activity.SetState("Pop Snacks");
	activity.GetAssets().SetSmallImage("the");
	activity.GetAssets().SetSmallText("i mage");
	activity.GetAssets().SetLargeImage("the");
	activity.GetAssets().SetLargeText("u mage");
	activity.SetType(discord::ActivityType::Playing);
	mpCore->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
		UE_LOG(LogDiscord, Log, TEXT("%s updating activity!"),
			*FString((result == discord::Result::Ok) ? "Succeeded" : "Failed")
		);
	});
	
	return (DiscordResult)mLastResult;
}

bool UDiscordState::IsInitialized() const
{
	return mpCore != nullptr;
}

DiscordResult UDiscordState::PollCallbacks()
{
	INITIALIZED_GUARD("poll for discord updates")
	mLastResult = mpCore->RunCallbacks();
	return (DiscordResult)mLastResult;
}

void UDiscordState::outputLog(discord::LogLevel level, const char* message)
{
	switch (level)
	{
	case discord::LogLevel::Error:
		UE_LOG(LogDiscord, Error, TEXT("[DiscordSdk] %s"), message);
		break;
	case discord::LogLevel::Warn:
		UE_LOG(LogDiscord, Warning, TEXT("[DiscordSdk] %s"), message);
		break;
	case discord::LogLevel::Info:
		UE_LOG(LogDiscord, Log, TEXT("[DiscordSdk] %s"), message);
		break;
	case discord::LogLevel::Debug:
		UE_LOG(LogDiscord, Log, TEXT("[DiscordSdk] [DEBUG] %s"), message);
		break;
	}
}

DiscordResult UDiscordState::GetLastResult() const
{
	return (DiscordResult)mLastResult;
}

FDiscordUser UDiscordState::createUserFromDiscord(discord::User const &user) const
{
	return FDiscordUser {
			user.GetId(),
			FString(user.GetUsername()),
			FCString::Atoi(*FString(user.GetDiscriminator())),
			FString(user.GetAvatar()),
	};
}

FDiscordUser UDiscordState::GetCurrentUser(bool& valid) const
{
	valid = false;
	discord::User currentUser;
	auto result = mpCore->UserManager().GetCurrentUser(&currentUser);

	if (result != discord::Result::Ok)
	{
		return {};
	}

	valid = true;
	return createUserFromDiscord(currentUser);
}

void UDiscordState::onCurrentUserUpdate()
{
	bool userValid = false;
	auto user = this->GetCurrentUser(userValid);
	UE_LOG(LogDiscord, Log, TEXT("Updated current discord user to %s#%i"),
		*user.username, user.descriminator
	);
	if (userValid && this->EventOnUpdateCurrentUser.IsBound())
	{
		this->EventOnUpdateCurrentUser.Broadcast(user);
	}
}

DiscordResult UDiscordState::FetchUserById(int64 const id)
{
	INITIALIZED_GUARD("fetch user from id");
	mpCore->UserManager().GetUser(discord::UserId(id), this->bindDiscordCallback(&UDiscordState::onReceivedUserById));
	return DiscordResult::Ok; // TODO: tell user that action is in callback
}

void UDiscordState::onReceivedUserById(discord::Result result, discord::User const &user)
{
	if (result == discord::Result::Ok && this->EventOnReceivedUserById.IsBound())
	{
		this->EventOnReceivedUserById.Broadcast(createUserFromDiscord(user));
	}
}

DiscordResult UDiscordState::FetchUserAvatar(int64 const idUser, int32 const size)
{
	INITIALIZED_GUARD("fetch user image");

	discord::ImageHandle handle{};
	handle.SetId(idUser);
	handle.SetType(discord::ImageType::User);
	handle.SetSize(size);

	mpCore->ImageManager().Fetch(handle, true, this->bindDiscordCallback(&UDiscordState::onReceivedUserAvatar));

	return DiscordResult::Ok; // TODO: tell user that action is in callback
}

void UDiscordState::onReceivedUserAvatar(discord::Result res, discord::ImageHandle handle)
{
	if (res != discord::Result::Ok)
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to fetch avatar, error %i"), int32(res))
		return;
	}
	if (!this->EventOnReceivedUserAvatar.IsBound()) return;

	discord::ImageDimensions dims{};
	mpCore->ImageManager().GetDimensions(handle, &dims);

	std::vector<uint8_t> data;
	data.reserve(dims.GetWidth() * dims.GetHeight() * 4);
	uint8_t* d = data.data();
	mpCore->ImageManager().GetData(handle, d, data.size());

	TArray<uint8> imgData;
	imgData.SetNumUninitialized(dims.GetWidth() * dims.GetHeight() * 4);
	mpCore->ImageManager().GetData(handle, imgData.GetData(), imgData.Num());

	/*
	if (res == discord::Result::Ok) {
		discord::ImageDimensions dims{};
		mpCore->ImageManager().GetDimensions(handle, &dims);

		std::vector<uint8_t> data;
		data.reserve(dims.GetWidth() * dims.GetHeight() * 4);
		uint8_t* d = data.data();
		mpCore->ImageManager().GetData(handle, d, data.size());

#if defined(_WIN32)
		auto fileSize =
			data.size() + sizeof(BitmapImageHeader) + sizeof(BitmapFileHeader);

		BitmapImageHeader imageHeader;
		imageHeader.width = static_cast<int32_t>(dims.GetWidth());
		imageHeader.height = static_cast<int32_t>(dims.GetHeight());

		BitmapFileHeader fileHeader;
		fileHeader.size = static_cast<uint32_t>(fileSize);

		FILE* fp = fopen("avatar.bmp", "wb");
		fwrite(&fileHeader, sizeof(BitmapFileHeader), 1, fp);
		fwrite(&imageHeader, sizeof(BitmapImageHeader), 1, fp);

		for (auto y = 0u; y < dims.GetHeight(); ++y) {
			auto pixels = reinterpret_cast<uint32_t const*>(data.data());
			auto invY = dims.GetHeight() - y - 1;
			fwrite(
				&pixels[invY * dims.GetWidth()], sizeof(uint32_t) * dims.GetWidth(), 1, fp);
		}

		fflush(fp);
		fclose(fp);
#endif
	}
	else {
		std::cout << "Failed fetching avatar. (err " << static_cast<int>(res) << ")\n";
	}
	//*/

	// Taken from: https://github.com/EpicGames/UnrealEngine/blob/4.11/Engine/Source/Editor/NewsFeed/Private/Implementation/NewsFeedCache.cpp#L167
	// per source https://answers.unrealengine.com/questions/351237/how-can-i-get-an-image-from-a-url-and-use-it-as-a.html

	TSharedPtr<FSlateDynamicImageBrush> Brush;

	uint32 BytesPerPixel = 4;
	int32 Width = 0;
	int32 Height = 0;

	bool bSucceeded = false;
	//TArray<uint8> DecodedImage;
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	
	/*
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(InRawData.GetData(), InRawData.Num()))
	{
		Width = ImageWrapper->GetWidth();
		Height = ImageWrapper->GetHeight();

		const TArray<uint8>* RawData = NULL;

		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
		{
			DecodedImage.AddUninitialized(Width * Height * BytesPerPixel);
			DecodedImage = *RawData;
			bSucceeded = true;
		}
	}
	//*/

	FName resourceName = FName(*FString::Printf(TEXT("%lld"), handle.GetId()));

	if (FSlateApplication::Get().GetRenderer()->GenerateDynamicImageResource(
		resourceName,
		dims.GetWidth(), dims.GetHeight(), imgData
	))
	{
		auto brush = FSlateDynamicImageBrush(resourceName, FVector2D(dims.GetWidth(), dims.GetHeight()));
		this->EventOnReceivedUserAvatar.Broadcast(brush);
	}

}
