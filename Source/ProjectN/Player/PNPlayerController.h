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
	
	void SetLockOnTargetActor();
	FORCEINLINE void DisableLockOn() { LockOnTargetActor = nullptr; }

private:
	APNPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaSeconds) override final;
	virtual void BeginPlay() override final;

	bool CanLockOnTargetActor(AActor* TargetActor) const;

	UFUNCTION()
	void CheckLockOnTimerCallback();

private:
	UPROPERTY(EditDefaultsOnly, Category = LockOn, meta = (AllowPrivateAccess = "true"))
	float SearchLockOnTargetRadius = 1500.0f;

	FPNPercent LockOnTargetVisibleAreaRate;

	TObjectPtr<AActor> LockOnTargetActor = nullptr;

	FTimerHandle CheckLockOnTimerHandle;
	FTimerHandle ClearLockOnTargetTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = LockOn, meta = (AllowPrivateAccess = "true"))
	float CheckLockOnTimerPeriod = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = LockOn, meta = (AllowPrivateAccess = "true"))
	float ClearLockOnTimerPeriod = 3.0f;
};
