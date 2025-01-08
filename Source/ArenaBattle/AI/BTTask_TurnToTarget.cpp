// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"

#include "ABAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	check(Owner);

	IABCharacterAIInterface* OwnerAIInterface = Cast<IABCharacterAIInterface>(Owner);
	check(OwnerAIInterface);

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - Owner->GetActorLocation();
	LookVector.Z = 0; // 수평 방향으로만 회전하기 위함
	const FRotator TargetRotator = FRotationMatrix::MakeFromX(LookVector).Rotator(); // X축 방향으로 하는 회전 행렬을 만들어 회전 값(Rotator)로 변환
	Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), TargetRotator, GetWorld()->GetDeltaSeconds(), OwnerAIInterface->GetAIAITurnSpeed())); // 현재 Rotator에서 TargetRotator로 회전 속도에 따라 부드럽게 보간

	return EBTNodeResult::Succeeded;
}
