// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNPawnSensingComponent.h"

#include "PNActorExtensionComponent.h"
#include "PNCommonModule.h"
#include "PNPercent.h"
#include "PNStatusActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

constexpr float DefaultDetectAngle = 45.0f;
constexpr float DefaultSightRadius = 10.0f * Meter;
constexpr float LockOnSightRadius = 10.0f * Meter;

constexpr uint8 GridDivisionCount = 10;
constexpr uint32 TotalGridPoints = GridDivisionCount * GridDivisionCount;

void UPNPawnSensingComponent::ConvertSenseType(const ESenseType InDetectType)
{
	switch (InDetectType)
	{
	case ESenseType::Default:
		{
			SightRadius = DefaultSightRadius;
			break;
		}
	case ESenseType::LockOn:
		{
			SightRadius = LockOnSightRadius;
			break;
		}
	}

	UpdateAISensing();
}

UPNPawnSensingComponent::UPNPawnSensingComponent()
{
	bWantsInitializeComponent = true;

	SightRadius = DefaultSightRadius;
	bOnlySensePlayers = true;
}

void UPNPawnSensingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetPeripheralVisionAngle(DefaultDetectAngle);
}

void UPNPawnSensingComponent::UpdateAISensing()
{
	APawn* Owner = GetOwner<APawn>();
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OverlappingActors;
	const FVector OwnerLocation = Owner->GetActorLocation();

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), OwnerLocation, SightRadius, TArray<TEnumAsByte<EObjectTypeQuery>>(), APawn::StaticClass(), ActorsToIgnore, OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		SensePawn(*Cast<APawn>(Actor));
	}
}

void UPNPawnSensingComponent::SensePawn(APawn& Pawn)
{
	if (bSeePawns && ShouldCheckVisibilityOf(&Pawn) && CouldSeePawn(&Pawn, false))
	{
		BroadcastOnSeePawn(Pawn);
	}
}

bool UPNPawnSensingComponent::ShouldCheckVisibilityOf(APawn* Pawn) const
{
	if (!Super::ShouldCheckVisibilityOf(Pawn))
	{
		return false;
	}

	const bool bPawnIsPlayer = (Pawn->Controller && Pawn->Controller->PlayerState);
	if (bOnlySenseMonster && bPawnIsPlayer)
	{
		return false;
	}

	return true;
}

void UPNPawnSensingComponent::SetTimer(const float TimeDelay)
{
	APawn* Owner = GetOwner<APawn>();
	const UPNActorExtensionComponent* ActorExtensionComponent = Owner->FindComponentByClass<UPNActorExtensionComponent>();
	
	// 플레이어는 서버에서 적을 탐지할 필요가 없기 때문에 클라이언트에서 탐지
	if (ActorExtensionComponent->IsPlayerActor())
	{
		if (IsClientActor(Owner))
		{
			Owner->GetWorldTimerManager().SetTimer(TimerHandle_OnTimer, this, &UPNPawnSensingComponent::OnTimer, TimeDelay, false);
		}
	}
	else
	{
		Super::SetTimer(TimeDelay);
	}
}

bool UPNPawnSensingComponent::CouldSeePawn(const APawn* Other, bool bMaySkipChecks) const
{
	if (!Super::CouldSeePawn(Other, bMaySkipChecks))
	{
		return false;
	}

	APawn* Owner = GetOwner<APawn>();
	if (Other == Owner)
	{
		return false;
	}

	UPNStatusActorComponent* StatusActorComponent = Other->FindComponentByClass<UPNStatusActorComponent>();
	if (StatusActorComponent == nullptr)
	{
		return false;
	}

	if (StatusActorComponent->IsDead())
	{
		return false;
	}

	// 일단 캡슐 컴포넌트만 가능, 추후 변경 가능
	UCapsuleComponent* CapsuleComponent = Other->FindComponentByClass<UCapsuleComponent>();
	if (CapsuleComponent == nullptr)
	{
		return false;
	}

	const FVector OwnerPawnLocation = Owner->GetActorLocation();
	const FVector TargetLocation = Other->GetActorLocation();
	FVector StartLocation = OwnerPawnLocation;
	FVector SelfForwardVector = Owner->GetActorForwardVector();

	if (Owner->IsPlayerControlled())
	{
		const APlayerCameraManager* CameraManager = Cast<APlayerController>(Owner->GetController())->PlayerCameraManager;
		StartLocation = CameraManager->GetCameraLocation();
		SelfForwardVector = CameraManager->GetActorForwardVector();
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

	FThreadSafeCounter TotalPointCounter(0);
	FThreadSafeCounter VisiblePointCounter(0);

#ifdef ENABLE_DRAW_DEBUG
	TMap<FVector, bool> CheckPointHits;
	FCriticalSection CheckPointHitsCriticalSection;
	CheckPointHits.Reserve(TotalGridPoints);
#endif

	ParallelFor(TotalGridPoints,
	            [this, SelfForwardVector, ScaledHalfHeight, ScaledRadius, BoxHeight, BoxWidth, RightVector, UpVector, &ComponentTransform, StartLocation, &TotalPointCounter, &VisiblePointCounter, &QueryParams
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

		            TotalPointCounter.Increment();

		            const FVector DirectionSelfToPoint = (Point - StartLocation).GetSafeNormal();
		            const float CosAngleSelfToPoint = FVector::DotProduct(SelfForwardVector, DirectionSelfToPoint);
		            if (CosAngleSelfToPoint < PeripheralVisionCosine)
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

	const FPNPercent SenseVisibleAreaPercent(30);
	return SenseVisibleAreaPercent <= FPNPercent::FromFraction(VisiblePointCounter.GetValue(), TotalPointCounter.GetValue());
}

void UPNPawnSensingComponent::SetPlayerSensor(const float InCameraFOVAngle)
{
	bOnlySensePlayers = false;
	bOnlySenseMonster = true;

	SetPeripheralVisionAngle(InCameraFOVAngle);
}
