#include "EquipmentDataTable.h"

#include "AbilitySystem/AttributeSet/PNAttributeSet.h"
#include "Engine/AssetManager.h"

UClass* FEquipmentDataTable::GetWeaponAttributeSetClass() const
{
	return WeaponAttributeSetPath.ResolveClass();
}

void FEquipmentDataTable::PostLoadDataTable()
{
	UAssetManager::GetStreamableManager().RequestAsyncLoad(WeaponAttributeSetPath);
}
