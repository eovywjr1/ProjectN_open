// Fill out your copyright notice in the Description page of Project Settings.


#include "PNBTDecorator_InRange.h"

#include "PNCommonModule.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/PNAI.h"

bool UPNBTDecorator_InRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const float TempInRange = 2.0f * Meter;

	if (UObject* Enemy = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_ENEMY))
	{
		AActor* EnemyCast = Cast<AActor>(Enemy);
		AActor* Owner = OwnerComp.GetOwner();

		return Owner->GetDistanceTo(EnemyCast) <= TempInRange;
	}

	return true;
}
