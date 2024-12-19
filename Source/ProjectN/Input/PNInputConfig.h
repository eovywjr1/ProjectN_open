
#pragma once

#include "GameplayTagContainer.h"
#include "PNInputConfig.generated.h"

class UInputAction;

USTRUCT()
struct FPNInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};

UCLASS(Const)
class UPNInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPNInputConfig(const FObjectInitializer& ObjectInitializer);

	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FPNInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPNInputAction> AbilityInputActions;
};