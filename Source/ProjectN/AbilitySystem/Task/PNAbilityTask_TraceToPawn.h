// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCommonModule.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "PNAbilityTask_TraceToPawn.generated.h"

class APNTargetActor_HitCheckActor_MultiSweep;

DECLARE_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&);

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNAbilityTask_TraceToPawn : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	static UPNAbilityTask_TraceToPawn* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class APNTargetActor_HitCheckActor> TargetActorClass, const FHitBoxData& InHitBoxData);
	
private:
	virtual void Activate() override final;
	virtual void OnDestroy(bool bInOwnerFinished) override final;
	virtual void TickTask(float DeltaTime) override final;
	
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	
public:
	FTraceResultDelegate OnComplete;
	
private:
	UPROPERTY()
	TSubclassOf<APNTargetActor_HitCheckActor> TargetActorClass;
	
	UPROPERTY()
	TObjectPtr<APNTargetActor_HitCheckActor> SpawnedTargetActor;
	
	FHitBoxData HitBoxData;
	TSet<TWeakObjectPtr<AActor>> HitActors;
	float ElapsedTime = 0.0f;
};
