// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNSkillComponent.h"

#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"
#include "Interface/PNAbilitySystemInterface.h"

void UPNSkillComponent::ClearCombo()
{
	check(RootComboNode.IsValid());

	CurrentComboNode = RootComboNode;
}

const FAttackData* UPNSkillComponent::ExecuteNextCombo(const FGameplayTag NextAttackTag)
{
	check(CurrentComboNode.IsValid());
	check(HasAuthority());

	if (!IsEnableNextCombo(NextAttackTag))
	{
		return nullptr;
	}

	TWeakPtr<FComboNode>* NextComboNode = CurrentComboNode.Pin()->Children.Find(NextAttackTag);
	CurrentComboNode = *NextComboNode;

	return CurrentComboNode.Pin()->ComboData;
}

bool UPNSkillComponent::IsEnableNextCombo(const FGameplayTag NextAttackTag) const
{
	TWeakPtr<FComboNode>* NextComboNode = CurrentComboNode.Pin()->Children.Find(NextAttackTag);
	if (NextComboNode == nullptr || !NextComboNode->IsValid())
	{
		return false;
	}

	const FAttackData* NextComboData = NextComboNode->Pin()->ComboData;
	if (NextComboData->GameplayEffect)
	{
		const UGameplayEffect* AttackGameplayEffect = NextComboData->GameplayEffect->GetDefaultObject<UGameplayEffect>();
		UAbilitySystemComponent* AbilitySystemComponent = Cast<IAbilitySystemInterface>(GetOwner())->GetAbilitySystemComponent();
		if (!AbilitySystemComponent->CanApplyAttributeModifiers(AttackGameplayEffect, 1, AbilitySystemComponent->MakeEffectContext()))
		{
			return false;
		}
	}

	return true;
}

bool UPNSkillComponent::IsCurrentCombo(const FGameplayTag AttackTag)
{
	if (!CurrentComboNode.IsValid())
	{
		ClearCombo();
		return false;
	}

	const FAttackData* CurrentComboData = CurrentComboNode.Pin()->ComboData;
	return CurrentComboData && CurrentComboData->AttackTag.MatchesTagExact(AttackTag);
}

UPNSkillComponent::UPNSkillComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;

	RootComboNode = CreateNode(nullptr);
	CurrentComboNode = RootComboNode;
}

void UPNSkillComponent::InitComboTree()
{
	check(RootComboNode.IsValid());

	const UPNWeaponAttributeSet* WeaponAttributeSet = GetOwner<IPNAbilitySystemInterface>()->GetAbilitySystemComponent()->GetSet<UPNWeaponAttributeSet>();
	check(WeaponAttributeSet);

	for (TArray<FComboData>::TConstIterator Iter = WeaponAttributeSet->GetComboDatas(); Iter; ++Iter)
	{
		FComboNode* CurrentNode = RootComboNode.Pin().Get();

		for (const FAttackData& AttackData : Iter->ComboAttackDatas)
		{
			TWeakPtr<FComboNode>* ChildComboNode = CurrentNode->Children.Find(AttackData.AttackTag);
			if (ChildComboNode == nullptr)
			{
				ChildComboNode = &CurrentNode->Children.Add(AttackData.AttackTag, CreateNode(&AttackData));
			}

			check(ChildComboNode->IsValid());
			CurrentNode = ChildComboNode->Pin().Get();
		}
	}
}

TWeakPtr<FComboNode> UPNSkillComponent::CreateNode(const FAttackData* InComboData)
{
	ComboNodes.Add(MakeShared<FComboNode>(InComboData));

	return ComboNodes.Last();
}
