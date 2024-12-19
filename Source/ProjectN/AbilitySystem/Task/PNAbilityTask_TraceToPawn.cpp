// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Task/PNAbilityTask_TraceToPawn.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/TargetActor/PNTargetActor_HitCheckActor.h"

void UPNAbilityTask_TraceToPawn::Activate()
{
	Super::Activate();

	bTickingTask = true;

	SpawnedTargetActor = Cast<APNTargetActor_HitCheckActor>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	check(SpawnedTargetActor);

	SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &ThisClass::OnTargetDataReadyCallback);

	UAbilitySystemComponent* AbilitySystemComponentPtr = AbilitySystemComponent.Get();
	check(AbilitySystemComponentPtr);
	const FTransform SpawnTransform = AbilitySystemComponentPtr->GetAvatarActor()->GetTransform();
	SpawnedTargetActor->FinishSpawning(SpawnTransform);

	AbilitySystemComponentPtr->SpawnedTargetActors.Push(SpawnedTargetActor);
	SpawnedTargetActor->SetAttackHitBoxData(HitBoxData);
	SpawnedTargetActor->StartTargeting(Ability);

	SetWaitingOnAvatar();
}

void UPNAbilityTask_TraceToPawn::OnDestroy(bool bInOwnerFinished)
{
	if (IsValid(SpawnedTargetActor))
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UPNAbilityTask_TraceToPawn::TickTask(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	if (HitBoxData.HitBoxDurationTime < ElapsedTime)
	{
		FGameplayAbilityTargetData_ActorArray* ActorArrayTargetData = new FGameplayAbilityTargetData_ActorArray();
		ActorArrayTargetData->SetActors(HitActors.Array());
		FGameplayAbilityTargetDataHandle TargetDataHandle;
		TargetDataHandle.Add(ActorArrayTargetData);
		OnComplete.Broadcast(TargetDataHandle);

		bTickingTask = false;
		EndTask();
		
		return;
	}

	SpawnedTargetActor->ConfirmTargetingAndContinue();
}

UPNAbilityTask_TraceToPawn* UPNAbilityTask_TraceToPawn::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<APNTargetActor_HitCheckActor> TargetActorClass, const FHitBoxData& InHitBoxData)
{
	UPNAbilityTask_TraceToPawn* NewTask = NewAbilityTask<UPNAbilityTask_TraceToPawn>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	NewTask->HitBoxData = InHitBoxData;
	return NewTask;
}

void UPNAbilityTask_TraceToPawn::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (TargetDataHandle.IsValid(0) == false)
	{
		return;
	}

	HitActors.Append(TargetDataHandle.Get(0)->GetActors());
}
