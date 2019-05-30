#include "SrcPlayerState.h"
#include "Net/UnrealNetwork.h"

int32 ASrcPlayerState::GetNetworkId() const
{
	return this->PlayerId;
}

void ASrcPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASrcPlayerState, idDiscord);
	DOREPLIFETIME(ASrcPlayerState, idRpgCharacter);
}
