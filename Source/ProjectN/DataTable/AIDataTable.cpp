
#include "AIDataTable.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Engine/AssetManager.h"

UBehaviorTree* FAIDataTable::GetBehaviorTree() const
{
	return Cast<UBehaviorTree>(BehaviorTreePath.ResolveObject());
}

UBlackboardData* FAIDataTable::GetBlackBoard() const
{
	return Cast<UBlackboardData>(BlackBoardPath.ResolveObject());
}

void FAIDataTable::PostLoadDataTable()
{
	TArray<FSoftObjectPath> LoadAssets{BehaviorTreePath, BlackBoardPath};
	UAssetManager::GetStreamableManager().RequestAsyncLoad(LoadAssets);
}
