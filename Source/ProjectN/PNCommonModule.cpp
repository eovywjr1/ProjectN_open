#include "PNCommonModule.h"

bool IsServerActor(const AActor* Actor)
{
	if (Actor == nullptr)
	{
		return false;
	}
	
	return Actor->GetNetMode() != NM_Client;
}

bool IsClientActor(const AActor* Actor)
{
	if (Actor == nullptr)
	{
		return false;
	}

	return Actor->GetNetMode() != NM_DedicatedServer;
}
