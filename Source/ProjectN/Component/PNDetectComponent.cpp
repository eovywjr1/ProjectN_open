// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNDetectComponent.h"

#include "PNCommonModule.h"
#include "PNInteractionComponent.h"
#include "PNPawnSensingComponent.h"
#include "AI/PNAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/PNHUD.h"

constexpr float InteractDetectDistance = 2.0f * Meter;

#ifdef WITH_EDITOR
static TAutoConsoleVariable<bool> InteractionDetectRangeDrawDebug(TEXT("InteractionDetectRangeDrawDebug"), false, TEXT(""), ECVF_Default);
#endif

bool UPNDetectComponent::CanDetectEnemy(const APawn* Enemy) const
{
	UPNPawnSensingComponent* SensingComponent = GetOwner()->FindComponentByClass<UPNPawnSensingComponent>();
	return SensingComponent->CouldSeePawn(Enemy);
}

bool UPNDetectComponent::IsDetectedEnemy(const APawn* Enemy) const
{
	return Enemy && DetectedEnemies.Find(Enemy);
}

UPNDetectComponent::UPNDetectComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UPNDetectComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* Owner = GetOwner<APawn>();
	if (IsServerActor(Owner) && Owner->IsPlayerControlled())
	{
		if (!CanDetectEnemy(TargetedEnemy))
		{
			SetTargetNextPriorityEnemy();
		}

		DetectInteractableActor();
	}
}

void UPNDetectComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (UPNPawnSensingComponent* SensingComponent = GetOwner()->FindComponentByClass<UPNPawnSensingComponent>())
	{
		SensingComponent->OnSeePawn.AddDynamic(this, &ThisClass::OnSeePawn);
	}
}

void UPNDetectComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TargetedEnemy);
}

void UPNDetectComponent::SetTargetNextPriorityEnemy()
{
	UpdateDetectedPawns();
	
	if (DetectedEnemies.IsEmpty())
	{
		return;
	}

	TArray<APawn*> DetectedEnemyArray = DetectedEnemies.Array();
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	DetectedEnemyArray.Sort([OwnerLocation](const AActor& A, const AActor& B)
	{
		float DistancePlayerToA = FVector::Dist(OwnerLocation, A.GetActorLocation());
		float DistancePlayerToB = FVector::Dist(OwnerLocation, B.GetActorLocation());
		return DistancePlayerToA < DistancePlayerToB;
	});

	if (CanDetectEnemy(TargetedEnemy) == false)
	{
		SetTargetEnemy(DetectedEnemyArray[0]);
		return;
	}

	const uint32 DetectedEnemyArraySize = DetectedEnemyArray.Num();
	for (uint32 index = 0; index < DetectedEnemyArraySize; ++index)
	{
		if (TargetedEnemy == DetectedEnemyArray[index])
		{
			SetTargetEnemy(DetectedEnemyArray[(index + 1) % DetectedEnemyArraySize]);
			break;
		}
	}
}

void UPNDetectComponent::SetTargetEnemy(APawn* InDetectedEnemy)
{
	check(IsServerActor(GetOwner()));

	if (TargetedEnemy == InDetectedEnemy)
	{
		return;
	}

	TargetedEnemy = InDetectedEnemy;

	OnDetectedDelegate.Broadcast();
}

void UPNDetectComponent::DetectInteractableActor() const
{
	APawn* Owner = GetOwner<APawn>();
	if (!Owner->IsPlayerControlled())
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
				ClientDetectInteractableActor(HitActor, InteractionDataTableKey);

				break;
			}
		}
	}

	if (!bDetectedInteractableActor)
	{
		ClientDetectInteractableActor(nullptr, NAME_None);
	}
}

void UPNDetectComponent::ClientDetectInteractableActor_Implementation(AActor* DetectActor, FName InteractionDataTableKey) const
{
	APNHUD* HUD = Cast<APNHUD>(GetOwner<APawn>()->GetController<APlayerController>()->GetHUD());
	if (DetectActor)
	{
		HUD->OnDetectedInteractableActorDelegate.Broadcast(DetectActor, InteractionDataTableKey);
	}
	else
	{
		HUD->OnUnDetectedInteractableActorDelegate.Broadcast();
	}
}

void UPNDetectComponent::OnSeePawn(APawn* Pawn)
{
	DetectedEnemies.Add(Pawn);
}

void UPNDetectComponent::UpdateDetectedPawns()
{
	if (DetectedEnemies.IsEmpty())
	{
		SetTargetEnemy(nullptr);
		return;
	}

	TArray<APawn*> EnemiesToRemove;
	for (auto Iter = DetectedEnemies.CreateConstIterator(); Iter; ++Iter)
	{
		if (!CanDetectEnemy(*Iter))
		{
			EnemiesToRemove.Add(*Iter);
		}
	}

	for (APawn* EnemyToRemove : EnemiesToRemove)
	{
		DetectedEnemies.Remove(EnemyToRemove);
	}
}
