// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterMonster.h"

#include "AI/PNAIController.h"
#include "Component/PNActorExtensionComponent.h"
#include "DataTable/MonsterDataTable.h"
#include "Subsystem/PNGameDataSubsystem.h"

APNCharacterMonster::APNCharacterMonster()
{
	AIControllerClass = APNAIController::StaticClass();
}

void APNCharacterMonster::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	CreateActorComponent(EActorType::Monster);
}

UPNActorGameData* APNCharacterMonster::GetActorGameData() const
{
	// Todo. 추후 스폰할 때 키로 변경해야 함
	const FMonsterDataTable* MonsterDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FMonsterDataTable>(TEXT("0"));
	check(MonsterDataTable);

	UPNActorGameData* ActorGameData = Cast<UPNActorGameData>(MonsterDataTable->GetMonsterGameData());
	
	return ActorGameData;
}
