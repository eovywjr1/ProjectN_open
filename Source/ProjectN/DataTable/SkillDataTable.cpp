
#include "SkillDataTable.h"

#include "GameplayEffect.h"
#include "AbilitySystem/AttributeSet/PNPlayerAttributeSet.h"

void FSkillDataTable::ApplySkillModifiers(UGameplayEffect& InSkillEffect) const
{
	{
		FGameplayModifierInfo StatusModifierInfo;
		StatusModifierInfo.Attribute = UPNPawnAttributeSet::GetAttackDamageAttribute();
		StatusModifierInfo.ModifierOp = EGameplayModOp::Override;
		StatusModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(GetSkillDamage()));
		InSkillEffect.Modifiers.Add(StatusModifierInfo);
	}

	{
		FGameplayModifierInfo StatusModifierInfo;
		StatusModifierInfo.Attribute = UPNPlayerAttributeSet::GetSRAttribute();
		const int8 SkillEffectSRAmount = GetUseSkillResource() * -1;
		StatusModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(SkillEffectSRAmount));
		InSkillEffect.Modifiers.Add(StatusModifierInfo);
	}
}

void FSkillDataTable::ApplyPostSkillModifiers(UGameplayEffect& InSkillEffect) const
{
	{
		FGameplayModifierInfo StatusModifierInfo;
		StatusModifierInfo.Attribute = UPNPlayerAttributeSet::GetSRAttribute();
		StatusModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(GetGetSkillResource()));
		InSkillEffect.Modifiers.Add(StatusModifierInfo);
	}
}
