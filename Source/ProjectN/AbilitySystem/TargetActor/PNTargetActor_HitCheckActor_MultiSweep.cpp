// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TargetActor/PNTargetActor_HitCheckActor_MultiSweep.h"

#include "Chaos/Private/Chaos/CollisionOneShotManifolds.h"

FGameplayAbilityTargetDataHandle APNTargetActor_HitCheckActor_MultiSweep::MakeTargetData() const
{
	TArray<TWeakObjectPtr<AActor>> HitActors;

	switch (AttackHitBoxData.Shape)
	{
	case EHitBoxShape::Cone:
		{
			break;
		}
	case EHitBoxShape::Box:
		{
			GetHitBoxActors(HitActors);

			break;
		}
	}

	FGameplayAbilityTargetData_ActorArray* ActorArrayTargetData = new FGameplayAbilityTargetData_ActorArray();
	ActorArrayTargetData->SetActors(HitActors);
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(ActorArrayTargetData);

	return TargetDataHandle;
}

void APNTargetActor_HitCheckActor_MultiSweep::GetHitBoxActors(TArray<TWeakObjectPtr<AActor>>& OutHitActors) const
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(SourceActor);

	// Todo. 소켓 위치로 변경해야 함
	const FVector Start = SourceActor->GetActorLocation();
	const FCollisionShape CollisionShape = FCollisionShape::MakeBox(AttackHitBoxData.BoxExtent);

	GetWorld()->SweepMultiByChannel(HitResults, Start, Start, SourceActor->GetActorRotation().Quaternion(), ECC_Pawn, CollisionShape, QueryParams);

#ifdef ENABLE_DRAW_DEBUG
	DrawDebugBox(GetWorld(), Start, AttackHitBoxData.BoxExtent, SourceActor->GetActorRotation().Quaternion(), FColor::Green, false, 5.0f);
#endif

	HitResults.RemoveAll([this](const FHitResult& Hit)
	{
		return Cast<APawn>(Hit.GetActor()) == nullptr;
	});

	for (const FHitResult& Hit : HitResults)
	{
		OutHitActors.Add(Hit.GetActor());
	}
}
