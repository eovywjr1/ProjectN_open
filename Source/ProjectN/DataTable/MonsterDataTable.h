#pragma once

#include "CoreMinimal.h"
#include "PNDataTable.h"
#include "Actor/PNPawnGameData.h"
#include "MonsterDataTable.Generated.h"

struct FAIDataTable;

USTRUCT()
struct FMonsterDataTable : public FPNDataTable
{
	GENERATED_USTRUCT_BODY()

public:
	const FAIDataTable* GetAIDataTable(const UObject* WorldContextObject) const;
	UPNPawnGameData* GetMonsterGameData() const;
	
private:
	virtual void PostLoadDataTable() override final;

private:
	UPROPERTY(EditDefaultsOnly)
	FName AIDataTableKey;
	
	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath MonsterGameDataPath;
};
