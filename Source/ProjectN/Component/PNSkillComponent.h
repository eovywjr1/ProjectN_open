// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"
#include "Components/PawnComponent.h"
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
class PROJECTN_API UPNSkillComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	void OnEquipWeapon();
	void OnAIPossessed();

	FORCEINLINE void ClearCombo();
	bool IsEnableNextCombo(const FGameplayTag NextAttackTag) const;
	FORCEINLINE bool IsCurrentCombo(const FGameplayTag AttackTag);
	const FAttackData* RequestNextCombo(const FGameplayTag NextAttackTag);
	
	UFUNCTION(Server, Reliable)
	void ServerClearCombo();
	
	UFUNCTION(Server, Reliable)
	void ServerExecuteNextCombo(const FGameplayTag NextAttackTag);
	
	UFUNCTION(Server, Reliable)
	void ServerPostSkillProcess(const bool bHit);
	
	bool IsEnableSkill(const FGameplayTag InputTag) const;
	
private:
	UPNSkillComponent(const FObjectInitializer& ObjectInitializer);

	TWeakPtr<FComboNode> CreateNode(const FAttackData* InComboData);
	void InitComboTree();
	const TArray<FComboData>* GetPlayerComboDatas();
	const TArray<FComboData>* GetMonsterComboDatas() const;
	
	bool IsEnableAttack(const FAttackData* AttackData) const;
	bool TryNextCombo(const FGameplayTag NextAttackTag);

private:
	// 콤보에는 공격과 스킬이 모두 포함되어 있음
	TArray<TSharedPtr<FComboNode>> ComboNodes;
	TWeakPtr<FComboNode> RootComboNode = nullptr;
	TWeakPtr<FComboNode> CurrentComboNode = nullptr;
};
