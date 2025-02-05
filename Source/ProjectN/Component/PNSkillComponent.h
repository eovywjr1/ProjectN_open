// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"
#include "PNSkillComponent.generated.h"

struct FAttackData;

struct FComboNode
{
public:
	FComboNode(const FAttackData* InComboData)
		: ComboData(InComboData)
	{}

	const FAttackData* ComboData = nullptr;
	TMap<FGameplayTag, TWeakPtr<FComboNode>> Children;
};

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void InitComboTree();
	FORCEINLINE void ClearCombo();
	const FAttackData* ExecuteNextCombo(const FGameplayTag NextAttackTag);
	bool IsEnableNextCombo(const FGameplayTag NextAttackTag) const;
	FORCEINLINE bool IsCurrentCombo(const FGameplayTag AttackTag);
	
private:
	UPNSkillComponent();

	TWeakPtr<FComboNode> CreateNode(const FAttackData* InComboData);

private:
	TArray<TSharedPtr<FComboNode>> ComboNodes;
	TWeakPtr<FComboNode> RootComboNode = nullptr;
	TWeakPtr<FComboNode> CurrentComboNode = nullptr;
};
