// Fill out your copyright notice in the Description page of Project Settings.


#include "PNGameDataSubsystem.h"

#include "Engine/DataTable.h"
#include "Engine/ObjectLibrary.h"

void UPNGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	LoadDataTable();
}

const UPNGameDataSubsystem* UPNGameDataSubsystem::Get()
{
	const UPNGameDataSubsystem* GameDataSubSystem = GEngine->GameViewport->GetGameInstance()->GetSubsystem<UPNGameDataSubsystem>();
	if (GameDataSubSystem == nullptr)
	{
		checkf(false, TEXT("호출되면 안됩니다. 클라에게 전달해주세요."));
		return nullptr;
	}

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
}
