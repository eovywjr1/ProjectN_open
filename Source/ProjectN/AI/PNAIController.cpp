// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PNAIController.h"

#include "PNAI.h"
#include "BehaviorTree/BlackboardComponent.h"

void APNAIController::OnDetectedEnemy(const AActor* Enemy)
{
	// 엔진 API의 non-const 필요로 const_cast를 강제로 사용
	GetBlackboardComponent()->SetValueAsObject(BBKEY_ENEMY, const_cast<AActor*>(Enemy));
}
