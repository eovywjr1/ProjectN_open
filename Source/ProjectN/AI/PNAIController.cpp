// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PNAIController.h"

#include "PNAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/PNEquipmentComponent.h"
#include "DataTable/AIDataTable.h"
#include "DataTable/MonsterDataTable.h"
#include "Subsystem/PNGameDataSubsystem.h"

void APNAIController::OnDetectedEnemy(const AActor* Enemy)
{
	if (Blackboard)
	{
		// 엔진 API의 non-const 필요로 const_cast를 강제로 사용
		Blackboard->SetValueAsObject(BBKEY_ENEMY, const_cast<AActor*>(Enemy));
	}
}

void APNAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Todo. 테스트 용도로 데이터테이블 키 하드코딩함, 추후 스폰할 때 가져와야 함
	MonsterDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FMonsterDataTable>(TEXT("0"));
	if (MonsterDataTable)
	{
		if (const FAIDataTable* AIDataTable = MonsterDataTable->GetAIDataTable(GetWorld()))
		{
			UBlackboardData* BlackBoard = AIDataTable->GetBlackBoard();
			UBehaviorTree* BehaviorTree = AIDataTable->GetBehaviorTree();

			if (BlackBoard && BehaviorTree)
			{
				UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
				if (UseBlackboard(BlackBoard, BlackboardComponent))
				{
					RunBehaviorTree(BehaviorTree);
				}
			}
		}
		
		GetPawn()->FindComponentByClass<UPNEquipmentComponent>()->RequestEquip(MonsterDataTable->GetWeaponItemKey());
	}
}
