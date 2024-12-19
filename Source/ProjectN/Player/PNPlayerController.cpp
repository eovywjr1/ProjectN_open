// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PNPlayerController.h"

#include "PNCheatManager.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

constexpr uint8 GridDivisionCount = 10;
constexpr uint32 TotalGridPoints = GridDivisionCount * GridDivisionCount;

APNPlayerController::APNPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  LockOnTargetVisibleAreaRate(30)
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
		const FVector TargetLocation = LockOnTargetActor->GetActorLocation();
		const FRotator NewRotation = (TargetLocation - PlayerCameraManager->GetCameraLocation()).Rotation();
		SetControlRotation(NewRotation);

#ifdef ENABLE_DRAW_DEBUG
		DrawDebugPoint(GetWorld(), TargetLocation, 40.0f, FColor::White, false, -1.0f);
#endif
	}
}

void APNPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
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

void APNPlayerController::SetLockOnTargetActor()
{
	APawn* OwnerPawn = GetPawn();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	TArray<AActor*> OverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	const FVector OwnerLocation = OwnerPawn->GetActorLocation();
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), OwnerLocation, SearchLockOnTargetRadius, TArray<TEnumAsByte<EObjectTypeQuery>>(), APawn::StaticClass(), ActorsToIgnore, OverlappingActors);

#ifdef ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(), OwnerLocation, SearchLockOnTargetRadius, 32, FColor::Red, true, 1.0f);
#endif

	AActor* NearestTarget = nullptr;
	float NearestDistance = FLT_MAX;

	for (AActor* Actor : OverlappingActors)
	{
		if (CanLockOnTargetActor(Actor) == false)
		{
			continue;
		}

		const float Distance = FVector::Distance(OwnerLocation, Actor->GetActorLocation());

		if (NearestTarget == nullptr)
		{
			NearestTarget = Actor;
			NearestDistance = Distance;
		}
		else if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestTarget = Actor;
		}
	}

	LockOnTargetActor = NearestTarget;

	if (LockOnTargetActor && CheckLockOnTimerHandle.IsValid() == false)
	{
		GetWorld()->GetTimerManager().SetTimer(CheckLockOnTimerHandle, this, &ThisClass::CheckLockOnTimerCallback, CheckLockOnTimerPeriod, false, CheckLockOnTimerPeriod);
	}
}

bool APNPlayerController::CanLockOnTargetActor(AActor* TargetActor) const
{
	if (IsValid(TargetActor) == false)
	{
		return false;
	}

	// Todo. 공격 판정 / 죽음 구현 후 체크해야 함
	// if (TargetActor->Dead())
	// {
	// 	return false;
	// }

	APawn* OwnerPawn = GetPawn();
	if (OwnerPawn == nullptr || TargetActor == OwnerPawn)
	{
		return false;
	}

	if (PlayerCameraManager == nullptr)
	{
		return false;
	}

	// 일단 캡슐 컴포넌트만 가능, 추후 변경 가능
	UCapsuleComponent* CapsuleComponent = TargetActor->FindComponentByClass<UCapsuleComponent>();
	if (CapsuleComponent == nullptr)
	{
		return false;
	}

	const FVector OwnerPawnLocation = OwnerPawn->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();
	if (SearchLockOnTargetRadius < FVector::Distance(OwnerPawnLocation, TargetLocation))
	{
		return false;
	}

	const FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	const FTransform ComponentTransform = CapsuleComponent->GetComponentTransform();
	const FVector CameraToTarget = (TargetLocation - CameraLocation).GetSafeNormal();

	const FVector RightVector = FVector::CrossProduct(FVector::UpVector, CameraToTarget).GetSafeNormal();
	const FVector UpVector = FVector::CrossProduct(CameraToTarget, RightVector).GetSafeNormal();

	const float ScaledRadius = CapsuleComponent->GetScaledCapsuleRadius();
	const float ScaledHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();

	const float BoxHeight = ScaledHalfHeight * 2.0f / GridDivisionCount;
	const float BoxWidth = (ScaledRadius * 2) / GridDivisionCount;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerPawn);

	const float FOVRadians = FMath::DegreesToRadians(PlayerCameraManager->GetFOVAngle());
	const float CosHalfFOV = FMath::Cos(FOVRadians * 0.5f);

	TArray<FVector> CheckPoints;
	FCriticalSection BoxCenterCriticalSection;
	CheckPoints.Reserve(TotalGridPoints);

	FThreadSafeCounter TotalPointCounter(0);
	
	ParallelFor(TotalGridPoints,
	            [this, ScaledHalfHeight, ScaledRadius, BoxHeight, BoxWidth, RightVector, UpVector, ComponentTransform, CameraLocation, CosHalfFOV, &CheckPoints, &BoxCenterCriticalSection, &TotalPointCounter](int32 Index)
	            {
		            const int32 HeightIndex = Index / GridDivisionCount;
		            const int32 WidthIndex = Index % GridDivisionCount;

		            const float VerticalOffset = -ScaledHalfHeight + (BoxHeight / 2) + (BoxHeight * HeightIndex);
		            const float HorizontalOffset = -ScaledRadius + (BoxWidth / 2) + (BoxWidth * WidthIndex);
		            
	            	FVector Point = ComponentTransform.GetLocation();
		            Point += UpVector * VerticalOffset + RightVector * HorizontalOffset;

		            const FVector LocalPoint = ComponentTransform.InverseTransformPosition(Point);
		            if (ScaledHalfHeight < FMath::Abs(LocalPoint.Z) || ScaledRadius < FMath::Sqrt(FMath::Square(LocalPoint.X) + FMath::Square(LocalPoint.Y)))
		            {
			            return;
		            }

		            TotalPointCounter.Increment();

		            const FVector DirectionCameraToPoint = (Point - CameraLocation).GetSafeNormal();
		            const float CosAngleCameraToBox = FVector::DotProduct(PlayerCameraManager->GetActorForwardVector(), DirectionCameraToPoint);
		            if (CosAngleCameraToBox < CosHalfFOV)
		            {
			            return;
		            }

		            {
			            FScopeLock Lock(&BoxCenterCriticalSection);
			            CheckPoints.Add(Point);
		            }
	            });

	uint8 VisiblePointCount = 0;

	for (const FVector& Point : CheckPoints)
	{
		FHitResult HitResult;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Point, CameraLocation, ECC_Visibility, QueryParams);
		if (bHit == false)
		{
			++VisiblePointCount;
		}

		#ifdef ENABLE_DRAW_DEBUG
				const FColor PointColor = bHit == false ? FColor::Green : FColor::Red;
				DrawDebugPoint(GetWorld(), Point, 5.0f, PointColor, false, 5.0f);
		#endif
	}

	return LockOnTargetVisibleAreaRate <= FPNPercent::FromFraction(VisiblePointCount, TotalPointCounter.GetValue());
}

void APNPlayerController::CheckLockOnTimerCallback()
{
	CheckLockOnTimerHandle.Invalidate();

	if (CanLockOnTargetActor(LockOnTargetActor))
	{
		GetWorld()->GetTimerManager().SetTimer(CheckLockOnTimerHandle, this, &ThisClass::CheckLockOnTimerCallback, CheckLockOnTimerPeriod, false, CheckLockOnTimerPeriod);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ClearLockOnTargetTimerHandle, [this]() { LockOnTargetActor = nullptr; }, ClearLockOnTimerPeriod, false, ClearLockOnTimerPeriod);
	}
}
