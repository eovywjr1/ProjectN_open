// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PNAIController.generated.h"

struct FMonsterDataTable;
/**
 * 
 */
UCLASS()
class PROJECTN_API APNAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	virtual void OnPossess(APawn* InPawn) override final;
	
	UFUNCTION()
	void OnDetectedEnemy();
	
private:
	const FMonsterDataTable* MonsterDataTable = nullptr;
};
