// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNSkillComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"

void UPNSkillComponent::ClearCombo()
{
	check(RootComboNode.IsValid());

	CurrentComboNode = RootComboNode;
}

const FAttackData* UPNSkillComponent::ExecuteNextCombo(const FGameplayTag NextAttackTag)
{
	check(CurrentComboNode.IsValid());

	TWeakPtr<FComboNode>* NextComboNode = CurrentComboNode.Pin()->Children.Find(NextAttackTag);
	if (NextComboNode == nullptr || !NextComboNode->IsValid())
	{
		return nullptr;
	}

	CurrentComboNode = *NextComboNode;

	return CurrentComboNode.Pin()->ComboData;
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

UPNSkillComponent::UPNSkillComponent()
{
	RootComboNode = CreateNode(nullptr);
	CurrentComboNode = RootComboNode;
}

void UPNSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	check(RootComboNode.IsValid());

	if (IAbilitySystemInterface* OwnerAbilitySystemInterface = GetOwner<IAbilitySystemInterface>())
	{
		if (const UPNWeaponAttributeSet* WeaponAttributeSet = OwnerAbilitySystemInterface->GetAbilitySystemComponent()->GetSet<UPNWeaponAttributeSet>())
		{
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
	}
}

TWeakPtr<FComboNode> UPNSkillComponent::CreateNode(const FAttackData* InComboData)
{
	ComboNodes.Add(MakeShared<FComboNode>(InComboData));

	return ComboNodes.Last();
}
