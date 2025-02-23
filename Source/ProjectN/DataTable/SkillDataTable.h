#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "SkillDataTable.Generated.h"

class UGameplayEffect;

USTRUCT()
struct FSkillDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	void ApplySkillModifiers(UGameplayEffect& InSkillEffect) const;
	void ApplyPostSkillModifiers(UGameplayEffect& InSkillEffect) const;

	FORCEINLINE uint8 GetSkillDamage() const { return SkillDamage; }
	FORCEINLINE uint8 GetGetSkillResource() const { return GetSkillResource; }
	FORCEINLINE uint8 GetUseSkillResource() const { return UseSkillResource; }
	
private:
	UPROPERTY(EditDefaultsOnly)
	uint8 SkillDamage = 0;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 GetSkillResource = 0;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 UseSkillResource = 0;
};
