// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PNDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
private:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override final;
};
