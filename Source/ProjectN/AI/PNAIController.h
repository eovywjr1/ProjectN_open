// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PNAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API APNAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void OnDetectedEnemy(const AActor* Enemy);
};
