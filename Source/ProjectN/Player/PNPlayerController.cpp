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

void APNPlayerController::ActivateLockOn(const bool bIsActivate)
{
	if (bIsActivate == false)
	{
		LockOnTargetActor = nullptr;
		CheckLockOnTimerHandle.Invalidate();
		return;
	}

	TArray<AActor*> SortedLockOnTargetActor;
	GetSortedLockOnTargetActor(SortedLockOnTargetActor);
	if (SortedLockOnTargetActor.IsEmpty())
	{
		return;
	}

	SetLockOnTargetActor(SortedLockOnTargetActor[0]);
}

void APNPlayerController::SetNextPriorityLockOnTargetActor()
{
	if (IsValid(LockOnTargetActor) == false)
	{
		return;
	}

	TArray<AActor*> SortedLockOnTargetActor;
	GetSortedLockOnTargetActor(SortedLockOnTargetActor);

	const uint32 SortedLockOnTargetActorNum = SortedLockOnTargetActor.Num();
	for (uint32 i = 0; i < SortedLockOnTargetActorNum; ++i)
	{
		if (LockOnTargetActor == SortedLockOnTargetActor[i])
		{
			SetLockOnTargetActor(SortedLockOnTargetActor[(i + 1) % SortedLockOnTargetActorNum]);
			break;
		}
	}
}

void APNPlayerController::SetLockOnTargetActor(const AActor* const InLockOnTargetActor)
{
	if (IsValid(InLockOnTargetActor) == false)
	{
		return;
	}

	LockOnTargetActor = InLockOnTargetActor;
	GetWorld()->GetTimerManager().SetTimer(CheckLockOnTimerHandle, this, &ThisClass::CheckLockOnTimerCallback, CheckLockOnTimerPeriod, false, CheckLockOnTimerPeriod);
}

void APNPlayerController::GetSortedLockOnTargetActor(TArray<AActor*>& InLockOnTargetActors) const
{
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OverlappingActors;
	const FVector OwnerLocation = GetPawn()->GetActorLocation();
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), OwnerLocation, SearchLockOnTargetRadius, TArray<TEnumAsByte<EObjectTypeQuery>>(), APawn::StaticClass(), ActorsToIgnore, OverlappingActors);

#ifdef ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(), OwnerLocation, SearchLockOnTargetRadius, 32, FColor::Red, true, 1.0f);
#endif

	InLockOnTargetActors.Reserve(OverlappingActors.Num());

	for (AActor* Actor : OverlappingActors)
	{
		if (CanLockOnTargetActor(Actor) == false)
		{
			continue;
		}

		InLockOnTargetActors.Add(Actor);
	}

	InLockOnTargetActors.Shrink();
	InLockOnTargetActors.Sort([OwnerLocation](const AActor& A, const AActor& B)
	{
		float DistancePlayerToA = FVector::Dist(OwnerLocation, A.GetActorLocation());
		float DistancePlayerToB = FVector::Dist(OwnerLocation, B.GetActorLocation());
		return DistancePlayerToA < DistancePlayerToB;
	});
}

bool APNPlayerController::CanLockOnTargetActor(const AActor* TargetActor) const
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
	if (TargetActor == OwnerPawn)
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

	FThreadSafeCounter TotalPointCounter(0);
	FThreadSafeCounter VisiblePointCounter(0);

#ifdef ENABLE_DRAW_DEBUG
	TMap<FVector, bool> CheckPointHits;
	FCriticalSection CheckPointHitsCriticalSection;
	CheckPointHits.Reserve(TotalGridPoints);
#endif

	ParallelFor(TotalGridPoints,
	            [this, ScaledHalfHeight, ScaledRadius, BoxHeight, BoxWidth, RightVector, UpVector, &ComponentTransform, CameraLocation, CosHalfFOV, &TotalPointCounter, &VisiblePointCounter, &QueryParams
#ifdef ENABLE_DRAW_DEBUG
		          , &CheckPointHits, &CheckPointHitsCriticalSection
#endif
	            ](int32 Index)
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

		            FHitResult HitResult;
		            const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Point, CameraLocation, ECC_Visibility, QueryParams);
		            if (bHit == false)
		            {
			            VisiblePointCounter.Increment();
		            }

#ifdef ENABLE_DRAW_DEBUG
		            {
			            FScopeLock Lock(&CheckPointHitsCriticalSection);
			            CheckPointHits.Add(Point, bHit);
		            }
#endif
	            });

// #ifdef ENABLE_DRAW_DEBUG
// 	for (const TPair<FVector, bool>& CheckPointHit : CheckPointHits)
// 	{
// 		const FColor PointColor = CheckPointHit.Value == false ? FColor::Green : FColor::Red;
// 		DrawDebugPoint(GetWorld(), CheckPointHit.Key, 5.0f, PointColor, false, 5.0f);
// 	}
// #endif

	return LockOnTargetVisibleAreaRate <= FPNPercent::FromFraction(VisiblePointCounter.GetValue(), TotalPointCounter.GetValue());
}

void APNPlayerController::CheckLockOnTimerCallback()
{
	if (CheckLockOnTimerHandle.IsValid() == false)
	{
		return;
	}

	if (CanLockOnTargetActor(LockOnTargetActor))
	{
		GetWorld()->GetTimerManager().SetTimer(CheckLockOnTimerHandle, this, &ThisClass::CheckLockOnTimerCallback, CheckLockOnTimerPeriod, false, CheckLockOnTimerPeriod);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ClearLockOnTargetTimerHandle, [this]() { LockOnTargetActor = nullptr; }, ClearLockOnTimerPeriod, false, ClearLockOnTimerPeriod);
	}
}
