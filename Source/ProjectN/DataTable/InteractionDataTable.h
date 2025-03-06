#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PNDataTable.h"
#include "InteractionDataTable.Generated.h"


USTRUCT()
struct FInteractionDataTable : public FPNDataTable
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
