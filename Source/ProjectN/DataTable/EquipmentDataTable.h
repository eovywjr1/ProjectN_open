#pragma once

#include "CoreMinimal.h"
#include "PNDataTable.h"
#include "EquipmentDataTable.Generated.h"

class UPNAttributeSet;

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
	UPNAttributeSet* GetWeaponAttributeSet() const;
	
	
private:
	virtual void PostLoadDataTable() override final;

private:
	UPROPERTY(EditDefaultsOnly)
	EEquipSlotType EquipSlotType;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> StatusKeys;
	
	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath WeaponAttributeSetPath;
};
