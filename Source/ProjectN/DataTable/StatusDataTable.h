#pragma once

#include "CoreMinimal.h"
#include "PNDataTable.h"
#include "StatusDataTable.Generated.h"

UENUM()
enum class EStatusType : uint8
{
	Invalid,
	Power,
	Hp,
	MaxHp,
	SR
};

USTRUCT()
struct FStatusDataTable : public FPNDataTable
{
	GENERATED_USTRUCT_BODY()

public:
	FORCEINLINE const EStatusType GetStatusType() const { return StatusType; }
	FORCEINLINE const float GetValue() const { return Value; }

private:
	UPROPERTY(EditDefaultsOnly)
	EStatusType StatusType;
	
	UPROPERTY(EditDefaultsOnly)
	float Value;
};
