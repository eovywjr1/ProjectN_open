#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PNCommonModule.generated.h"

constexpr float CheckDetectEnemyPeriod = 1.0f;
constexpr uint16 Meter = 100;
constexpr uint16 DefaultMeasurementUnit = 100 * Meter;

UENUM(BlueprintType)
enum class EHitBoxShape : uint8
{
	Box,
	Cone
};

USTRUCT(BlueprintType)
struct PROJECTN_API FHitBoxData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	EHitBoxShape Shape;

	UPROPERTY(EditDefaultsOnly)
	float HitBoxDurationTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Shape == EHitBoxShape::Cone", EditConditionHides))
	float Radius = 0.0f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Shape == EHitBoxShape::Cone", EditConditionHides))
	float Angle = 0.0f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Shape == EHitBoxShape::Box", EditConditionHides))
	FVector BoxExtent = FVector::ZeroVector;
};

USTRUCT()
struct FAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackTag;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AttackActionMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FName SkillDataTableIndex = NAME_None;
	
	UPROPERTY(EditDefaultsOnly)
	FName AttackActionMontageSectionName = NAME_None;
};

USTRUCT()
struct FComboData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAttackData> ComboAttackDatas;
};

bool IsServerActor(const AActor* Actor);
bool IsClientActor(const AActor* Actor);