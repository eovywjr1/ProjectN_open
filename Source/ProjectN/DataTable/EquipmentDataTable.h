#pragma once

#include "CoreMinimal.h"
#include "PNDataTable.h"
#include "EquipmentDataTable.Generated.h"

UENUM()
enum class EEquipSlotType : uint8
{
	Invalid,
	Weapon,
	Head,
	Body,
	Hand,
	Shoes
};

USTRUCT()
struct FEquipmentDataTable : public FPNDataTable
{
	GENERATED_USTRUCT_BODY()

public:
	FORCEINLINE EEquipSlotType GetEquipSlotType() const { return EquipSlotType; }
	const TArray<FName> GetStatusKeys() const { return StatusKeys; }

private:
	UPROPERTY(EditDefaultsOnly)
	EEquipSlotType EquipSlotType;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> StatusKeys;
};
