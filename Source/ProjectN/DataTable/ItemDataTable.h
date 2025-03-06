#pragma once

#include "CoreMinimal.h"
#include "PNDataTable.h"
#include "ItemDataTable.Generated.h"

UENUM()
enum class EItemType : uint8
{
	Invalid,
	Equipment,
	Gear,
	Consumables,
	Chipset,
	Collection
};

// Todo. ItemGrade는 임시
UENUM()
enum class EItemGrade : uint8
{
	Invalid,
	Normal,
	Epic,
	Unique,
	Legendary
};

USTRUCT()
struct FItemDataTable : public FPNDataTable
{
	GENERATED_USTRUCT_BODY()

public:
	FORCEINLINE FName GetEquipmentKey() const { return EquipmentKey; }

private:
	UPROPERTY(EditDefaultsOnly)
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly)
	EItemGrade ItemGrade;

	UPROPERTY(EditDefaultsOnly)
	FName EquipmentKey;
};
