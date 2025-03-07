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
	const FAIDataTable* GetAIDataTable(const UObject* WorldContextObject) const;
	FORCEINLINE FName GetWeaponItemKey() const { return WeaponItemKey; }

private:
	UPROPERTY(EditDefaultsOnly)
	FName AIDataTableKey;
	
	UPROPERTY(EditDefaultsOnly)
	FName WeaponItemKey;
};
