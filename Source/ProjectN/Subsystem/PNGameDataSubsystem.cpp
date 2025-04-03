// Fill out your copyright notice in the Description page of Project Settings.


#include "PNGameDataSubsystem.h"

#include "DataTable/PNDataTable.h"
#include "Engine/ObjectLibrary.h"

void UPNGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	LoadDataTable();
}

const UPNGameDataSubsystem* UPNGameDataSubsystem::Get(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	check(World);
	
	const UPNGameDataSubsystem* GameDataSubSystem = UGameInstance::GetSubsystem<UPNGameDataSubsystem>(World->GetGameInstance());
	check(GameDataSubSystem);

	return GameDataSubSystem;
}

void UPNGameDataSubsystem::LoadDataTable()
{
	const FString DataTablePath = TEXT("/Game/ProjectN/DataTable");
	
	UObjectLibrary* ObjectLibrary = UObjectLibrary::CreateLibrary(UDataTable::StaticClass(), false, GIsEditor);
	ObjectLibrary->AddToRoot();
	ObjectLibrary->LoadAssetDataFromPath(DataTablePath);
	ObjectLibrary->LoadAssetsFromAssetData();

	TArray<FAssetData> DataTables;
	ObjectLibrary->GetAssetDataList(DataTables);

	for (const FAssetData& AssetData : DataTables)
	{
		const UDataTable* DataTable = Cast<UDataTable>(AssetData.GetAsset());
		if (DataTable == nullptr)
		{
			checkf(false, TEXT("데이터테이블(%s)이 유효하지 않습니다."), *AssetData.AssetName.ToString());
		}

		const UScriptStruct* DataTableStruct = DataTable->GetRowStruct();
		TMap<FName, TObjectPtr<const FTableRowBase>>& TypeMap = DataMap.FindOrAdd(DataTableStruct);
		
		DataTable->ForeachRow<FTableRowBase>(TEXT(""), [&TypeMap](const FName& RowName, const FTableRowBase& Data)
		{
			TypeMap.Add(RowName, &Data);
		});
	}
	
	PostLoadDataTables();
}

void UPNGameDataSubsystem::PostLoadDataTables()
{
	for (auto& DataMapTuple : DataMap)
	{
		TMap<FName, TObjectPtr<const FTableRowBase>>& DataTableMap = DataMapTuple.Value;
		for (auto& DataTableMapTuple : DataTableMap)
		{
			const FPNDataTable* DataTable = static_cast<const FPNDataTable*>(DataTableMapTuple.Value.Get());
			
			// 데이터테이블을 안전하게 소유하기 위해 const로 저장하고, 초기화가 필요한 곳에 const_cast를 사용 
			FPNDataTable* CastDataTable = const_cast<FPNDataTable*>(DataTable);
			CastDataTable->PostLoadDataTable();
			CastDataTable->SetRowName(DataTableMapTuple.Key);
		}
	}
}
