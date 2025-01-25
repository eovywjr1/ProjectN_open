#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InteractionDataTable.Generated.h"


USTRUCT()
struct FInteractionDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FORCEINLINE FName GetInteractionUIText() const { return InteractionUIText; }
	FORCEINLINE float GetInteractionKeyPressTime() const { return InteractionKeyPressTime; }

private:
	UPROPERTY(EditDefaultsOnly)
	FName InteractionUIText;
	
	UPROPERTY(EditDefaultsOnly)
	float InteractionKeyPressTime = 0.0f;
};
