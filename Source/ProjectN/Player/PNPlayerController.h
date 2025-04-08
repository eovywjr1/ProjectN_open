// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "PNPlayerController.generated.h"

struct FInputActionValue;

class UPNInputConfig;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class PROJECTN_API APNPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void EnableControlInput(bool bEnable) const;
	bool CanCameraInputControl() const;

	void RotationByInput(const FVector2D LookAxisVector);

	void ActivateLockOn(const bool bActivate);
	void SetNextPriorityLockOnTargetActor();
	
	void UpdateLockOnCameraRotation();

private:
	APNPlayerController();

	virtual void Tick(float DeltaSeconds) override final;
	virtual void SetupInputComponent() override final;
	virtual void OnPossess(APawn* InPawn) override final;
	virtual void OnRep_Pawn() override final;

	UFUNCTION()
	void CheckLockOnTimerCallback();
	
	void SetLockOnTarget();

	void LoadInputData();
	void Input_AbilityPressed(FGameplayTag InputTag);
	void Input_AbilityReleased(FGameplayTag InputTag);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LockOn(const FInputActionValue& InputActionValue);
	void Input_NextLockOnTarget(const FInputActionValue& InputActionValue);
	
	void ToggleInventory();

private:
	APawn* LockOnTargetActor = nullptr;

	FTimerHandle CheckLockOnTimerHandle;
	FTimerHandle ClearLockOnTargetTimerHandle;
	
	float LastRotationYaw = 0.0f;
	
	bool bIsActivatedLockOn = false;
	
	UPROPERTY()
	TObjectPtr<UPNInputConfig> InputConfig = nullptr;
	
	UPROPERTY()
	TObjectPtr<UInputMappingContext> ControlMappingContext = nullptr;
	
	UPROPERTY()
	TObjectPtr<UInputMappingContext> CameraMappingContext = nullptr;
	
	UPROPERTY()
	TObjectPtr<UInputMappingContext> UIInputMappingContext = nullptr;
};
