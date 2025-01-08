// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	check(Owner);

	IABCharacterAIInterface* OwnerAIInterface = Cast<IABCharacterAIInterface>(Owner);
	check(OwnerAIInterface);

	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda([this, &OwnerComp]() { FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); });
	OwnerAIInterface->SetAIAttackDelegate(OnAttackFinished);
	OwnerAIInterface->AttackByAI();

	return EBTNodeResult::InProgress;
}
