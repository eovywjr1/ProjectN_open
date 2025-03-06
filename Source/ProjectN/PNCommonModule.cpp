
#include "PNCommonModule.h"

bool IsServerActor(const AActor* Actor)
{
	return Actor->GetNetMode() != NM_Client;
}

bool IsClientActor(const AActor* Actor)
{
	return Actor->GetNetMode() != NM_DedicatedServer;
}
