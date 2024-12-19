// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TargetActor/PNTargetActor_HitCheckActor.h"

#include "Abilities/GameplayAbility.h"

void APNTargetActor_HitCheckActor::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void APNTargetActor_HitCheckActor::ConfirmTargetingAndContinue()
{
	FGameplayAbilityTargetDataHandle TargetDataHandle = MakeTargetData();
	TargetDataReadyDelegate.Broadcast(TargetDataHandle);
}
