#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "InteractionDataTable.Generated.h"


USTRUCT()
struct FInteractionDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FORCEINLINE FName GetUIText() const { return UIText; }
	FORCEINLINE float GetKeyPressTime() const { return KeyPressTime; }
	FORCEINLINE FGameplayTag GetInteractionAbilityTag() const { return InteractionAbilityTag; }

private:
	UPROPERTY(EditDefaultsOnly)
	FName UIText;
	 
	UPROPERTY(EditDefaultsOnly)
	float KeyPressTime = 0.0f;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InteractionAbilityTag;
};
