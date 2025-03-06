// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PNBTTask_PlayAbility.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "VisualLogger/VisualLogger.h"

EBTNodeResult::Type UPNBTTask_PlayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UPNBTTask_PlayAbility::ExecuteTask failed since AIController is missing."));
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* AbilitySystemComponent = AIController->GetPawn()->FindComponentByClass<UAbilitySystemComponent>();
	if (AbilitySystemComponent == nullptr)
	{
		UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UPNBTTask_PlayAbility::ExecuteTask failed since UAbilitySystemComponent is missing."));
		return EBTNodeResult::Failed;
	}
	
	const bool bSuccess = AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));

	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
