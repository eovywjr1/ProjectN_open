// Fill out your copyright notice in the Description page of Project Settings.


#include "PNBTDecorator_InRange.h"

#include "PNCommonModule.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/PNAI.h"

bool UPNBTDecorator_InRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UObject* Enemy = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_ENEMY))
	{
		AActor* EnemyCast = Cast<AActor>(Enemy);
		AActor* Owner = OwnerComp.GetOwner();

		const float Distance =  Owner->GetDistanceTo(EnemyCast);
		const float TempInRange = 10.0f * Meter;

		return Distance <= TempInRange;
	}

	return false;
}
