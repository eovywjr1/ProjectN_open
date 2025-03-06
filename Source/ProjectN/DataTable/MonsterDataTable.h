#pragma once

#include "CoreMinimal.h"
#include "PNDataTable.h"
#include "MonsterDataTable.Generated.h"

struct FAIDataTable;

USTRUCT()
struct FMonsterDataTable : public FPNDataTable
{
	GENERATED_USTRUCT_BODY()

public:
	FORCEINLINE const FAIDataTable* GetAIDataTable(const UObject* WorldContextObject) const;

private:
	virtual void PostLoadDataTable() override final;

private:
	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath WeaponAttributeSetPath;
	
	UPROPERTY(EditDefaultsOnly)
	FName AIDataTableKey;
};
