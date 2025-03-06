// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PNBTTask_PlayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNBTTask_PlayAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override final;
	
private:
	UPROPERTY(EditAnywhere)
	FGameplayTag AbilityTag;
};
