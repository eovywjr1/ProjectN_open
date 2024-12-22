// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/PNPlayerAttributeSet.h"

#include "GameplayEffectExtension.h"

void UPNPlayerAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	// Todo. SP와 같이 플레이어 전용 스텟 추가

	Super::PostGameplayEffectExecute(Data);
}
