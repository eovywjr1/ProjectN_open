// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNPercent.h"
#include "GameFramework/PlayerController.h"
#include "PNPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API APNPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	bool CanCameraInputControl() const;

	void RotationByInput(const FVector2D LookAxisVector);

	void ActivateLockOn();
	
	UFUNCTION(Server, Reliable)
	void ServerSetActivateLockOn(bool bActivate);
	
	void DeActivateLockOn();
	
	UFUNCTION(Server, Reliable)
	void ServerSetNextPriorityLockOnTargetActor();
	
	void UpdateLockOnCameraRotation();
	
	UFUNCTION(Server, Unreliable)
	void ServerUpdateControlRotation(FRotator Rotation);

private:
	APNPlayerController();

	virtual void Tick(float DeltaSeconds) override final;
	virtual void SetupInputComponent() override final;
	virtual void OnPossess(APawn* InPawn) override final;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override final;

	UFUNCTION()
	void CheckLockOnTimerCallback();
	
	void SetLockOnTarget();
	
	UFUNCTION()
	void OnRep_LockOnTargetActor();

private:
	UPROPERTY(ReplicatedUsing = OnRep_LockOnTargetActor)
	APawn* LockOnTargetActor = nullptr;

	FTimerHandle CheckLockOnTimerHandle;
	FTimerHandle ClearLockOnTargetTimerHandle;
	
	float LastRotationYaw = 0.0f;
};
