#pragma once

#include "CoreMinimal.h"
#include "PNDataTable.h"
#include "AIDataTable.Generated.h"

class UBehaviorTree;
class UBlackboardData;

USTRUCT()
struct FAIDataTable : public FPNDataTable
{
	GENERATED_USTRUCT_BODY()

public:
	UBehaviorTree* GetBehaviorTree() const;
	UBlackboardData* GetBlackBoard() const;
	
private:
	virtual void PostLoadDataTable() override final;

private:
	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath BehaviorTreePath;
	
	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath BlackBoardPath;
};
