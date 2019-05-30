#include "ActorHelperLib.h"
#include "GameFramework/Actor.h"

void UActorHelperLib::SetActorEditorFolder(AActor *actor, FName const folder)
{
#if WITH_EDITOR
	actor->SetFolderPath(folder);
#endif
}

void UActorHelperLib::MatchActorEditorFolder(AActor *actor, AActor *sourceActor)
{
#if WITH_EDITOR
	SetActorEditorFolder(actor, sourceActor->GetFolderPath());
#endif
}

void UActorHelperLib::SetActorName(AActor *actor, FName const displayName)
{
	FString name = displayName.ToString();
	actor->Rename(*name);
#if WITH_EDITOR
	actor->SetActorLabel(*name);
#endif
}
