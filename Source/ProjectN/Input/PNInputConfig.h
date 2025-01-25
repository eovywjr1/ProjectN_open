
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
class UPNInputConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;
	
private:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override { return FPrimaryAssetId("PlayerInput", GetFName()); };

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FPNInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPNInputAction> AbilityInputActions;
};