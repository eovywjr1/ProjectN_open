// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"

#include "ABAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavigationSystemV1 = UNavigationSystemV1::GetNavigationSystem(Owner->GetWorld());
	if (NavigationSystemV1 == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* OwnerAIInterface = Cast<IABCharacterAIInterface>(Owner);
	if (OwnerAIInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	FVector Origin = BlackboardComponent->GetValueAsVector(BBKEY_HOMEPOS);
	FNavLocation NextPatrolPos;

	if (NavigationSystemV1->GetRandomReachablePointInRadius(Origin, OwnerAIInterface->GetAIPatrolRadius(), NextPatrolPos))
	{
		BlackboardComponent->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
