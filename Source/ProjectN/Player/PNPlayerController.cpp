// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PNPlayerController.h"

#include "PNCheatManager.h"
#include "Component/PNDetectComponent.h"
#include "UI/PNHUD.h"

constexpr float CheckLockOnTimerPeriod = 1.0f;
constexpr float ClearLockOnTimerPeriod = 3.0f;

APNPlayerController::APNPlayerController()
{
#if UE_WITH_CHEAT_MANAGER
	CheatClass = UPNCheatManager::StaticClass();
#endif
}

void APNPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(LockOnTargetActor))
	{
		FRotator NewRotation = GetControlRotation();
		NewRotation.Yaw = (LockOnTargetActor->GetActorLocation() - PlayerCameraManager->GetCameraLocation()).Rotation().Yaw;
		SetControlRotation(NewRotation);
	}
}

void APNPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

bool APNPlayerController::CanCameraInputControl() const
{
	if (IsValid(LockOnTargetActor))
	{
		return false;
	}

	return true;
}

void APNPlayerController::RotationByInput(const FVector2D LookAxisVector)
{
	if (CanCameraInputControl() == false)
	{
		return;
	}

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void APNPlayerController::ActivateLockOn()
{
	SetLockOnTarget();
}

void APNPlayerController::DeActivateLockOn()
{
	LockOnTargetActor = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(CheckLockOnTimerHandle);
	
	GetPawn()->FindComponentByClass<UPNDetectComponent>()->SetDetectTypeAndUpdateDetect(EDetectType::Default);

	Cast<APNHUD>(GetHUD())->OnDeactivatedLockOnDelegate.Broadcast();
}

void APNPlayerController::SetNextPriorityLockOnTargetActor()
{
	GetPawn()->FindComponentByClass<UPNDetectComponent>()->DetectNextPriorityEnemy();
	SetLockOnTarget();
}

void APNPlayerController::CheckLockOnTimerCallback()
{
	const AActor* LockOnTarget = GetPawn()->FindComponentByClass<UPNDetectComponent>()->GetDetectedEnemy();
	if (IsValid(LockOnTarget) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(ClearLockOnTargetTimerHandle, [this]() { DeActivateLockOn(); }, ClearLockOnTimerPeriod, false, ClearLockOnTimerPeriod);
	}
}

void APNPlayerController::SetLockOnTarget()
{
	UPNDetectComponent* DetectComponent = GetPawn()->FindComponentByClass<UPNDetectComponent>();
	DetectComponent->SetDetectTypeAndUpdateDetect(EDetectType::LockOn);
	const AActor* LockOnTarget = DetectComponent->GetDetectedEnemy();
	if (IsValid(LockOnTarget) == false)
	{
		return;
	}

	LockOnTargetActor = LockOnTarget;

	if (CheckLockOnTimerHandle.IsValid() == false)
	{
		GetWorld()->GetTimerManager().SetTimer(CheckLockOnTimerHandle, this, &ThisClass::CheckLockOnTimerCallback, CheckLockOnTimerPeriod, true, CheckLockOnTimerPeriod);
	}

	Cast<APNHUD>(GetHUD())->OnSetLockOnTargetDelegate.Broadcast(LockOnTargetActor);
}
