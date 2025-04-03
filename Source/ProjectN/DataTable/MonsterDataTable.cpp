#include "MonsterDataTable.h"

#include "Component/PNActorGameData.h"
#include "DataTable/AIDataTable.h"
#include "Engine/AssetManager.h"
#include "Subsystem/PNGameDataSubsystem.h"

const FAIDataTable* FMonsterDataTable::GetAIDataTable(const UObject* WorldContextObject) const
{
	return UPNGameDataSubsystem::Get(WorldContextObject)->GetData<FAIDataTable>(AIDataTableKey);
}

const UPNActorGameData* FMonsterDataTable::GetMonsterGameData() const
{
	UPNActorGameData* LoadedMonsterGameData = Cast<UPNActorGameData>(MonsterGameDataPath.ResolveObject());
	if (LoadedMonsterGameData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterDataTable[%s] MonsterGameData[%s] Loaded Failed"), *RowName.ToString(), *MonsterGameDataPath.GetAssetPathString());
		return nullptr;
	}

	return LoadedMonsterGameData;
}

void FMonsterDataTable::PostLoadDataTable()
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(MonsterGameDataPath);
}
