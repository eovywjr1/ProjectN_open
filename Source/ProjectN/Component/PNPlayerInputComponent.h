// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"

#include "PNPlayerInputComponent.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UPNInputConfig;

struct FGameplayTag;
struct FInputActionValue;

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNPlayerInputComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void EnableControlInput(bool bEnable) const;
	FVector2D GetLastMovementInput() const { return LastMovementInput; }

private:
	UPNPlayerInputComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitializeComponent() override final;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_NextLockOnTarget(const FInputActionValue& InputActionValue);
	void Input_LockOn(const FInputActionValue& InputActionValue);
	
	void Input_AbilityPressed(FGameplayTag InputTag);
	void Input_AbilityReleased(FGameplayTag InputTag);
	
private:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> ControlMappingContext = nullptr;
	
	UPROPERTY()
	TObjectPtr<UInputMappingContext> CameraMappingContext = nullptr;
	
	UPROPERTY()
	TObjectPtr<UPNInputConfig> InputConfig = nullptr;
	
	FVector2D LastMovementInput = FVector2D::ZeroVector;
	
	bool bIsActivatedLockOn = false;
};
