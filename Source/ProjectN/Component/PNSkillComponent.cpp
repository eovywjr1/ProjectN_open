// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNSkillComponent.h"

#include "PNActorExtensionComponent.h"
#include "PNEquipmentComponent.h"
#include "PNGameplayTags.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PNPlayerAttributeSet.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"
#include "DataTable/EquipmentDataTable.h"
#include "DataTable/ItemDataTable.h"
#include "DataTable/SkillDataTable.h"
#include "Interface/PNAbilitySystemInterface.h"
#include "Subsystem/PNGameDataSubsystem.h"

void UPNSkillComponent::ClearCombo()
{
	check(RootComboNode.IsValid());

	CurrentComboNode = RootComboNode;

	if (!HasAuthority())
	{
		ServerClearCombo();
	}
}

const FAttackData* UPNSkillComponent::RequestNextCombo(const FGameplayTag NextAttackTag)
{
	check(CurrentComboNode.IsValid());
	
	if (!IsEnableNextCombo(NextAttackTag))
	{
		return nullptr;
	}
	
	if (!HasAuthority())
	{
		const bool bExecuteNextCombo = TryNextCombo(NextAttackTag);
		if (!bExecuteNextCombo)
		{
			return nullptr;
		}
	}

	if (IsClientActor(GetOwner()))
	{
		ServerExecuteNextCombo(NextAttackTag);
	}

	return CurrentComboNode.Pin()->ComboData;
}

void UPNSkillComponent::ServerClearCombo_Implementation()
{
	ClearCombo();
}

void UPNSkillComponent::ServerExecuteNextCombo_Implementation(const FGameplayTag NextAttackTag)
{
	const bool bExecuteNextCombo = TryNextCombo(NextAttackTag);
	if (!bExecuteNextCombo)
	{
		return;
	}
	
	const FAttackData* CurrentComboData = CurrentComboNode.Pin()->ComboData;
	if (CurrentComboData && !CurrentComboData->SkillDataTableIndex.IsNone())
	{
		UGameplayEffect* SkillEffect = NewObject<UGameplayEffect>(this, FName(TEXT("SkillEffect")));
		SkillEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

		if (const FSkillDataTable* SkillDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FSkillDataTable>(CurrentComboData->SkillDataTableIndex))
		{
			SkillDataTable->ApplySkillModifiers(*SkillEffect);
		}

		UPNAbilitySystemComponent* AbilitySystemComponent = Cast<IPNAbilitySystemInterface>(GetOwner())->GetPNAbilitySystemComponent();
		AbilitySystemComponent->ApplyGameplayEffectToSelf(SkillEffect);
	}
}

bool UPNSkillComponent::IsEnableNextCombo(const FGameplayTag NextAttackTag) const
{
	TWeakPtr<FComboNode>* NextComboNode = CurrentComboNode.Pin()->Children.Find(NextAttackTag);
	if (NextComboNode == nullptr || !NextComboNode->IsValid())
	{
		return false;
	}

	if (!IsEnableAttack(NextComboNode->Pin()->ComboData))
	{
		return false;
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

void UPNSkillComponent::ServerPostSkillProcess_Implementation(const bool bHit)
{
	if (bHit)
	{
		const FAttackData* CurrentComboData = CurrentComboNode.Pin()->ComboData;
		if (CurrentComboData && !CurrentComboData->SkillDataTableIndex.IsNone())
		{
			UGameplayEffect* SkillEffect = NewObject<UGameplayEffect>(this, FName(TEXT("PostSkillEffect")));
			SkillEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

			if (const FSkillDataTable* SkillDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FSkillDataTable>(CurrentComboData->SkillDataTableIndex))
			{
				SkillDataTable->ApplyPostSkillModifiers(*SkillEffect);
			}

			UPNAbilitySystemComponent* AbilitySystemComponent = Cast<IPNAbilitySystemInterface>(GetOwner())->GetPNAbilitySystemComponent();
			AbilitySystemComponent->ApplyGameplayEffectToSelf(SkillEffect);
		}
	}
}

bool UPNSkillComponent::IsEnableSkill(const FGameplayTag InputTag) const
{
	if (!InputTag.IsValid() || RootComboNode == nullptr)
	{
		return false;
	}

	// 초기 테스트 용도로 하드 코딩
	// Todo. 스킬 창이 구현되면 스킬 창에서 스킬 키에 등록된 데이터를 가져와야 함
	const FGameplayTag InputSkillQTag = FPNGameplayTags::FindTagByString("InputTag.Attack.Skill.Q");
	const FGameplayTag InputSkillETag = FPNGameplayTags::FindTagByString("InputTag.Attack.Skill.E");
	const FGameplayTag InputSkillRTag = FPNGameplayTags::FindTagByString("InputTag.Attack.Skill.R");

	FGameplayTag AbilitySkillTag;
	const FAttackData* SkillData = nullptr;

	if (InputTag.MatchesTagExact(InputSkillQTag))
	{
		AbilitySkillTag = FPNGameplayTags::FindTagByString("Ability.Attack.Skill.TestQ");
	}
	else if (InputTag.MatchesTagExact(InputSkillETag))
	{
		AbilitySkillTag = FPNGameplayTags::FindTagByString("Ability.Attack.Skill.TestE");
	}
	else if (InputTag.MatchesTagExact(InputSkillRTag))
	{
		AbilitySkillTag = FPNGameplayTags::FindTagByString("Ability.Attack.Skill.TestR");
	}

	if (TWeakPtr<FComboNode>* SkillNode = RootComboNode.Pin()->Children.Find(AbilitySkillTag))
	{
		SkillData = SkillNode->Pin()->ComboData;
	}

	if (SkillData == nullptr)
	{
		return false;
	}

	if (!IsEnableAttack(SkillData))
	{
		return false;
	}

	return true;
}

UPNSkillComponent::UPNSkillComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UPNSkillComponent::OnEquipWeapon()
{
	InitComboTree();
}

void UPNSkillComponent::InitComboTree()
{
	ComboNodes.Empty();

	RootComboNode = CreateNode(nullptr);
	CurrentComboNode = RootComboNode;

	const UPNActorExtensionComponent* ActorExtensionComponent = GetOwner()->FindComponentByClass<UPNActorExtensionComponent>();
	if(ActorExtensionComponent->IsPlayerActor())
	{
		PlayerInitComboTree();
	}
	else if(ActorExtensionComponent->IsMonsterActor())
	{
		MonsterInitComboTree();
	}
}

void UPNSkillComponent::PlayerInitComboTree()
{
	const UPNEquipmentComponent* EquipmentComponent = GetOwner()->FindComponentByClass<UPNEquipmentComponent>();
	UClass* WeaponAttributeSetClass = EquipmentComponent->GetWeaponAttributeSetClass();
	if (WeaponAttributeSetClass == nullptr)
	{
		return;
	}

	UPNWeaponAttributeSet* WeaponAttributeSet = NewObject<UPNWeaponAttributeSet>(this, WeaponAttributeSetClass);
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

void UPNSkillComponent::MonsterInitComboTree()
{

}

TWeakPtr<FComboNode> UPNSkillComponent::CreateNode(const FAttackData* InComboData)
{
	ComboNodes.Add(MakeShared<FComboNode>(InComboData));

	return ComboNodes.Last();
}

bool UPNSkillComponent::IsEnableAttack(const FAttackData* AttackData) const
{
	if (AttackData == nullptr)
	{
		return false;
	}

	if (!AttackData->SkillDataTableIndex.IsNone())
	{
		if (const FSkillDataTable* SkillDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FSkillDataTable>(AttackData->SkillDataTableIndex))
		{
			UAbilitySystemComponent* AbilitySystemComponent = Cast<IAbilitySystemInterface>(GetOwner())->GetAbilitySystemComponent();
			const float CurrentSR = AbilitySystemComponent->GetNumericAttribute(UPNPlayerAttributeSet::GetSRAttribute());

			if (CurrentSR < SkillDataTable->GetUseSkillResource())
			{
				return false;
			}
		}
	}

	return true;
}

bool UPNSkillComponent::TryNextCombo(const FGameplayTag NextAttackTag)
{
	if (!IsEnableNextCombo(NextAttackTag))
	{
		return false;
	}

	TWeakPtr<FComboNode>* NextComboNode = CurrentComboNode.Pin()->Children.Find(NextAttackTag);
	CurrentComboNode = *NextComboNode;

	return true;
}
