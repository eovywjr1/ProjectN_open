// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	void UpdateHpBar(float CurrentHp);

private:
	virtual void NativeConstruct() override final;
	
private:
	UPROPERTY()
	TObjectPtr<class UABHpBarWidget> HpBar;
	
	UPROPERTY()
	TObjectPtr<class UABCharacterStatWidget> CharacterStat;
};
