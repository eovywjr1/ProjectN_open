#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "ABCharacterStat.generated.h"

USTRUCT()
struct FABCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterStat() : MaxHp(0.0f),
	                     Attack(0.0f),
	                     AttackRange(0.0f),
	                     AttackSpeed(0.0f),
	                     MovementSpeed(0.0f) {}
	                     
	// 속성들이 추가되거나 제거됐을 때 유연하게 작업하게 위해 float으로 통일
	// 속성 집합이 모두 float이므로 operator+를 수정할 필요가 없음
	FABCharacterStat operator+(const FABCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FABCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FABCharacterStat) / sizeof(float);

		for (int32 i = 0; i < StatNum; ++i)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}
		
		return Result;
	}
	
	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE float GetAttack() const { return Attack; }
	FORCEINLINE float GetAttackRange() const { return AttackRange; }
	FORCEINLINE float GetAttackSpeed() const { return AttackSpeed; }
	FORCEINLINE float GetMovementSpeed() const { return MovementSpeed; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float AttackSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float MovementSpeed;
};
