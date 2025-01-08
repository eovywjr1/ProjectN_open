// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNSkillComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"

const FAttackData* UPNSkillComponent::ExecuteNextCombo(const FGameplayTag NextAttackTag)
{
	FComboNode** NextComboNode = CurrentComboNode->Children.Find(NextAttackTag);
	if (NextComboNode == nullptr)
	{
		return nullptr;
	}

	CurrentComboNode = *NextComboNode;

	return CurrentComboNode->ComboData;
}

UPNSkillComponent::UPNSkillComponent()
{
	RootComboNode = CreateNode(nullptr);
	CurrentComboNode = RootComboNode;
}

void UPNSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IAbilitySystemInterface* OwnerAbilitySystemInterface = GetOwner<IAbilitySystemInterface>())
	{
		if (const UPNWeaponAttributeSet* WeaponAttributeSet = OwnerAbilitySystemInterface->GetAbilitySystemComponent()->GetSet<UPNWeaponAttributeSet>())
		{
			for(TArray<FComboData>::TConstIterator Iter = WeaponAttributeSet->GetComboDatas(); Iter; ++Iter)
			{
				FComboNode* CurrentNode = RootComboNode;

				for (const FAttackData& AttackData : Iter->ComboAttackDatas)
				{
					FComboNode** ChildComboNode = CurrentNode->Children.Find(AttackData.AttackTag);
					if (ChildComboNode == nullptr)
					{
						ChildComboNode = &CurrentNode->Children.Add(AttackData.AttackTag, CreateNode(&AttackData));
					}

					CurrentNode = *ChildComboNode;
				}
			}
		}
	}
}

FComboNode* UPNSkillComponent::CreateNode(const FAttackData* InComboData)
{
	ComboNodes.Add(MakeUnique<FComboNode>(InComboData));

	return ComboNodes.Last().Get();
}
