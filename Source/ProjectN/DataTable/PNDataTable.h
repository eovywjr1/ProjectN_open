#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PNDataTable.Generated.h"


USTRUCT()
struct FPNDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	virtual void PostLoadDataTable() {}

	FORCEINLINE void SetRowName(const FName InRowName) { RowName = InRowName; }

protected:
	FName RowName;
};
