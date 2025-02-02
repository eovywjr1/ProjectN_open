// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PNGameDataSubsystem.generated.h"

struct FTableRowBase;

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;

	template <typename T>
	const T* GetData(const FName& Key) const
	{
		static_assert(TIsDerivedFrom<T, FTableRowBase>::Value, "T must inherit from FTableRowBase");

		const TMap<FName, TObjectPtr<const FTableRowBase>>* TypeMap = DataMap.Find(T::StaticStruct());
		if (TypeMap == nullptr)
		{
			checkf(false, TEXT("%s 데이터테이블이 없습니다. 데이터테이블이 삭제됐는지 확인해주세요."), *T::StaticStruct()->GetFName().ToString());
			return nullptr;
		}

		const TObjectPtr<const FTableRowBase>* FoundData = TypeMap->Find(Key);
		if (FoundData == nullptr)
		{
			checkf(false, TEXT("%s 의 Row(%s)가 없습니다. 데이터가 삭제됐는지 확인해주세요."), *T::StaticStruct()->GetFName().ToString(), *Key.ToString());
			return nullptr;
		}

		return static_cast<const T*>(FoundData->Get());
	}

	static const UPNGameDataSubsystem* Get(const UObject* WorldContextObject);

private:
	void LoadDataTable();

	TMap<TObjectPtr<const UScriptStruct>, TMap<FName, TObjectPtr<const FTableRowBase>>> DataMap;
};

template <typename T>
bool IsValidDataTableKey(const FName Key)
{
	if (Key.IsValid() == false)
	{
		return false;
	}

	const T* DataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<T>(Key);
	if (DataTable == nullptr)
	{
		return false;
	}

	return true;
}
