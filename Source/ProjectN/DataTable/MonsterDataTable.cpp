
#include "MonsterDataTable.h"

#include "Engine/AssetManager.h"
#include "Subsystem/PNGameDataSubsystem.h"

const FAIDataTable* FMonsterDataTable::GetAIDataTable(const UObject* WorldContextObject) const
{
	return UPNGameDataSubsystem::Get(WorldContextObject)->GetData<FAIDataTable>(AIDataTableKey);
}

void FMonsterDataTable::PostLoadDataTable()
{
	TArray<FSoftObjectPath> LoadAssets{WeaponAttributeSetPath};
	UAssetManager::GetStreamableManager().RequestAsyncLoad(LoadAssets);
}
