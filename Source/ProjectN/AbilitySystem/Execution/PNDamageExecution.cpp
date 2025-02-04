// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Execution/PNDamageExecution.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PNPawnAttributeSet.h"

void UPNDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceAbilitySystemComponent && TargetAbilitySystemComponent)
	{
		const float SourcePower = SourceAbilitySystemComponent->GetNumericAttributeBase(UPNPawnAttributeSet::GetPowerAttribute());
		const float SourceAttackDamage = SourceAbilitySystemComponent->GetNumericAttributeBase(UPNPawnAttributeSet::GetAttackDamageAttribute());
		
		const float finalDamage = SourcePower + SourceAttackDamage;
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UPNPawnAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, finalDamage));
	}
}
