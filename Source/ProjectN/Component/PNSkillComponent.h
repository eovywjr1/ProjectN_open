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
	TMap<FGameplayTag, FComboNode*> Children;
};

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE void ClearCombo() { CurrentComboNode = RootComboNode; }
	const FAttackData* ExecuteNextCombo(const FGameplayTag NextAttackTag);
	FORCEINLINE bool IsCurrentCombo(const FGameplayTag AttackTag) const { return CurrentComboNode->ComboData && CurrentComboNode->ComboData->AttackTag.MatchesTagExact(AttackTag); }

private:
	UPNSkillComponent();
	virtual void BeginPlay() override final;

	FComboNode* CreateNode(const FAttackData* InComboData);

private:
	TArray<TUniquePtr<FComboNode>> ComboNodes;
	FComboNode* RootComboNode = nullptr;
	FComboNode* CurrentComboNode = nullptr;
};
