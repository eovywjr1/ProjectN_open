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
	void DeActivateLockOn();
	void SetNextPriorityLockOnTargetActor();

private:
	APNPlayerController();

	virtual void Tick(float DeltaSeconds) override final;
	virtual void BeginPlay() override final;

	UFUNCTION()
	void CheckLockOnTimerCallback();
	
	void SetLockOnTarget();

private:
	TObjectPtr<const AActor> LockOnTargetActor = nullptr;

	FTimerHandle CheckLockOnTimerHandle;
	FTimerHandle ClearLockOnTargetTimerHandle;
};
