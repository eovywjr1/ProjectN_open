// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNDetectComponent.h"

#include "PNCommonModule.h"
#include "PNInteractionComponent.h"
#include "PNPercent.h"
#include "PNStatusActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/PNHUD.h"

// 몬스터 감지 관련 기획이 나온 후 변경될 수 있음
constexpr float DefaultDetectAngle = 90.0f;
constexpr float DefaultCheckDetectEnemyDistance = 10.0f * static_cast<uint16>(EPNDistanceUnit::Meter);
constexpr float LockOnCheckDetectEnemyDistance = 10.0f * static_cast<uint16>(EPNDistanceUnit::Meter);
constexpr uint8 GridDivisionCount = 10;
constexpr uint32 TotalGridPoints = GridDivisionCount * GridDivisionCount;

constexpr float InteractDetectDistance = 2.0f * static_cast<uint16>(EPNDistanceUnit::Meter);

const FPNPercent DetectVisibleAreaRate(30);

#ifdef WITH_EDITOR
static TAutoConsoleVariable<bool> InteractionDetectRangeDrawDebug(TEXT("InteractionDetectRangeDrawDebug"), false, TEXT(""), ECVF_Default);
#endif

void UPNDetectComponent::SetDetectTypeAndUpdateDetect(const EDetectType InDetectType)
{
	switch (InDetectType)
	{
	case EDetectType::Default:
		{
			CheckDetectEnemyDistance = DefaultCheckDetectEnemyDistance;
			break;
		}
	case EDetectType::LockOn:
		{
			CheckDetectEnemyDistance = LockOnCheckDetectEnemyDistance;
		}
	}

	UpdateDetectedEnemy();
}

UPNDetectComponent::UPNDetectComponent()
	: CheckDetectEnemyDistance(DefaultCheckDetectEnemyDistance)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPNDetectComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* Owner = GetOwner<APawn>();
	if (Owner->IsLocallyControlled())
	{
		if (Owner->IsPlayerControlled())
		{
			DetectInteractableActor();
			UpdateDetectedEnemy();
		}
	}
}

void UPNDetectComponent::UpdateDetectedEnemy()
{
	if (IsDetectableEnemy(DetectedEnemy) == false)
	{
		TArray<const AActor*> InSortedDetectedEnemies;
		DetectEnemy(InSortedDetectedEnemies);

		SetDetectedEnemy(InSortedDetectedEnemies.IsEmpty() ? nullptr : InSortedDetectedEnemies[0]);
	}
}

void UPNDetectComponent::DetectEnemy(TArray<const AActor*>& InSortedDetectedEnemies) const
{
	AActor* Owner = GetOwner();
	check(IsValid(Owner));

	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OverlappingActors;
	const FVector OwnerLocation = Owner->GetActorLocation();

	// Todo. 적 타입 걸러야 함
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), OwnerLocation, CheckDetectEnemyDistance, TArray<TEnumAsByte<EObjectTypeQuery>>(), APawn::StaticClass(), ActorsToIgnore, OverlappingActors);

	InSortedDetectedEnemies.Reserve(OverlappingActors.Num());

	for (const AActor* Actor : OverlappingActors)
	{
		if (IsDetectableEnemy(Actor) == false)
		{
			continue;
		}

		InSortedDetectedEnemies.Add(Actor);
	}

	// 정렬 기준이 추후 변경될 수 있음 
	InSortedDetectedEnemies.Shrink();
	InSortedDetectedEnemies.Sort([OwnerLocation](const AActor& A, const AActor& B)
	{
		float DistancePlayerToA = FVector::Dist(OwnerLocation, A.GetActorLocation());
		float DistancePlayerToB = FVector::Dist(OwnerLocation, B.GetActorLocation());
		return DistancePlayerToA < DistancePlayerToB;
	});
}

void UPNDetectComponent::DetectNextPriorityEnemy()
{
	TArray<const AActor*> InSortedDetectedEnemies;
	DetectEnemy(InSortedDetectedEnemies);

	if (InSortedDetectedEnemies.IsEmpty())
	{
		return;
	}

	if (IsDetectableEnemy(DetectedEnemy) == false)
	{
		SetDetectedEnemy(InSortedDetectedEnemies[0]);
		return;
	}

	const uint32 SortedLockOnTargetActorNum = InSortedDetectedEnemies.Num();
	for (uint32 index = 0; index < SortedLockOnTargetActorNum; ++index)
	{
		if (DetectedEnemy == InSortedDetectedEnemies[index])
		{
			SetDetectedEnemy(InSortedDetectedEnemies[(index + 1) % SortedLockOnTargetActorNum]);
			break;
		}
	}
}

bool UPNDetectComponent::IsDetectableEnemy(const AActor* Enemy) const
{
	if (IsValid(Enemy) == false)
	{
		return false;
	}

	UPNStatusActorComponent* StatusActorComponent = Enemy->FindComponentByClass<UPNStatusActorComponent>();
	if(StatusActorComponent == nullptr)
	{
		return false;
	}
	
	if (StatusActorComponent->IsDead())
	{
		return false;
	}

	APawn* Owner = GetOwner<APawn>();
	check(Owner);

	if (Enemy == GetOwner())
	{
		return false;
	}

	// 일단 캡슐 컴포넌트만 가능, 추후 변경 가능
	UCapsuleComponent* CapsuleComponent = Enemy->FindComponentByClass<UCapsuleComponent>();
	if (CapsuleComponent == nullptr)
	{
		return false;
	}

	const FVector OwnerPawnLocation = Owner->GetActorLocation();
	const FVector TargetLocation = Enemy->GetActorLocation();
	if (CheckDetectEnemyDistance < FVector::Distance(OwnerPawnLocation, TargetLocation))
	{
		return false;
	}


	FVector StartLocation = OwnerPawnLocation;
	FVector SelfForwardVector = Owner->GetActorForwardVector();
	float DetectAngle = DefaultDetectAngle;

	if (Owner->IsPlayerControlled())
	{
		const APlayerCameraManager* CameraManager = Cast<APlayerController>(Owner->GetController())->PlayerCameraManager;
		StartLocation = CameraManager->GetCameraLocation();
		SelfForwardVector = CameraManager->GetActorForwardVector();
		DetectAngle = CameraManager->GetFOVAngle();
	}

	const FTransform ComponentTransform = CapsuleComponent->GetComponentTransform();
	const FVector SelfToEnemy = (TargetLocation - StartLocation).GetSafeNormal();

	const FVector RightVector = FVector::CrossProduct(FVector::UpVector, SelfToEnemy).GetSafeNormal();
	const FVector UpVector = FVector::CrossProduct(SelfToEnemy, RightVector).GetSafeNormal();

	const float ScaledRadius = CapsuleComponent->GetScaledCapsuleRadius();
	const float ScaledHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();

	const float BoxHeight = ScaledHalfHeight * 2.0f / GridDivisionCount;
	const float BoxWidth = (ScaledRadius * 2) / GridDivisionCount;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	const float DetectRadian = FMath::DegreesToRadians(DetectAngle);
	const float CosHalfDetectRadian = FMath::Cos(DetectRadian * 0.5f);

	FThreadSafeCounter TotalPointCounter(0);
	FThreadSafeCounter VisiblePointCounter(0);

#ifdef ENABLE_DRAW_DEBUG
	TMap<FVector, bool> CheckPointHits;
	FCriticalSection CheckPointHitsCriticalSection;
	CheckPointHits.Reserve(TotalGridPoints);
#endif

	ParallelFor(TotalGridPoints,
	            [this, SelfForwardVector, ScaledHalfHeight, ScaledRadius, BoxHeight, BoxWidth, RightVector, UpVector, &ComponentTransform, StartLocation, CosHalfDetectRadian, &TotalPointCounter, &VisiblePointCounter, &QueryParams
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

		            const FVector DirectionSelfToPoint = (Point - StartLocation).GetSafeNormal();
		            const float CosAngleSelfToPoint = FVector::DotProduct(SelfForwardVector, DirectionSelfToPoint);
		            if (CosAngleSelfToPoint < CosHalfDetectRadian)
		            {
			            return;
		            }

		            FHitResult HitResult;
		            const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Point, StartLocation, ECC_Visibility, QueryParams);
		            if (bHit == false)
		            {
			            VisiblePointCounter.Increment();
		            }

		            // #ifdef ENABLE_DRAW_DEBUG
		            // 		            {
		            // 			            FScopeLock Lock(&CheckPointHitsCriticalSection);
		            // 			            CheckPointHits.Add(Point, bHit);
		            // 		            }
		            // #endif
	            });

#ifdef ENABLE_DRAW_DEBUG
	for (const TPair<FVector, bool>& CheckPointHit : CheckPointHits)
	{
		const FColor PointColor = CheckPointHit.Value == false ? FColor::Green : FColor::Red;
		DrawDebugPoint(GetWorld(), CheckPointHit.Key, 5.0f, PointColor, false, 0.5f);
	}
#endif

	return DetectVisibleAreaRate <= FPNPercent::FromFraction(VisiblePointCounter.GetValue(), TotalPointCounter.GetValue());
}

void UPNDetectComponent::SetDetectedEnemy(const AActor* InDetectedEnemy)
{
	DetectedEnemy = InDetectedEnemy;

	if (IsValid(DetectedEnemy))
	{
		OnDetectedDelegate.Broadcast();
	}
}

void UPNDetectComponent::DetectInteractableActor() const
{
	APawn* Owner = GetOwner<APawn>();
	if (!Owner->Controller->IsPlayerController())
	{
		return;
	}

	constexpr float HalfInteractDetectDistance = InteractDetectDistance;
	const FVector SpherePosition = Owner->GetActorLocation() + Owner->GetActorForwardVector() * HalfInteractDetectDistance;
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), SpherePosition, SpherePosition, HalfInteractDetectDistance, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, false);

#ifdef ENABLE_DRAW_DEBUG
	if (InteractionDetectRangeDrawDebug->GetBool())
	{
		DrawDebugSphere(GetWorld(), SpherePosition, HalfInteractDetectDistance, 32, FColor::Green, false, 0.1f);
	}
#endif

	bool bDetectedInteractableActor = false;
	if (!OutHits.IsEmpty())
	{
		OutHits.Sort([OwnerLocation = Owner->GetActorLocation()](const FHitResult& A, const FHitResult& B)
		{
			float DistancePlayerToA = FVector::Dist(OwnerLocation, A.GetActor()->GetActorLocation());
			float DistancePlayerToB = FVector::Dist(OwnerLocation, B.GetActor()->GetActorLocation());
			return DistancePlayerToA < DistancePlayerToB;
		});

		for (const FHitResult HitResult : OutHits)
		{
			AActor* HitActor = HitResult.GetActor();
			UPNInteractionComponent* InteractComponent = HitActor->FindComponentByClass<UPNInteractionComponent>();
			check(InteractComponent);

			FName InteractionDataTableKey = InteractComponent->GetInteractionKey();
			if (!InteractionDataTableKey.IsNone())
			{
				bDetectedInteractableActor = true;
				Cast<APNHUD>(GetOwner<APawn>()->GetController<APlayerController>()->GetHUD())->OnDetectedInteractableActorDelegate.Broadcast(HitActor, InteractionDataTableKey);

				break;
			}
		}
	}

	if (!bDetectedInteractableActor)
	{
		Cast<APNHUD>(GetOwner<APawn>()->GetController<APlayerController>()->GetHUD())->OnUnDetectedInteractableActorDelegate.Broadcast();
	}
}
