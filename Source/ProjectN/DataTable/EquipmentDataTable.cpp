
#include "EquipmentDataTable.h"

#include "AbilitySystem/AttributeSet/PNAttributeSet.h"
#include "Engine/AssetManager.h"

UPNAttributeSet* FEquipmentDataTable::GetWeaponAttributeSet() const
{
	return Cast<UPNAttributeSet>(WeaponAttributeSetPath.ResolveObject());
}

void FEquipmentDataTable::PostLoadDataTable()
{
	TArray<FSoftObjectPath> LoadAssets{WeaponAttributeSetPath};
	UAssetManager::GetStreamableManager().RequestAsyncLoad(LoadAssets);
}
