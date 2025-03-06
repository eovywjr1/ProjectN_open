#pragma once

#include "CoreMinimal.h"
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

bool IsServerActor(const AActor* Actor);
bool IsClientActor(const AActor* Actor);