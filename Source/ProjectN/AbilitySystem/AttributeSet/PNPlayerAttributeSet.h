// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSet/PNPawnAttributeSet.h"
#include "PNPlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNPlayerAttributeSet : public UPNPawnAttributeSet
{
	GENERATED_BODY()
	
private:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override final;
	
};
