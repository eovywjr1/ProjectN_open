// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PNCheatManager.h"

#include "PNLogChannels.h"
#include "Component/PNInventoryComponent.h"

void UPNCheatManager::AddItem(FName ItemKey, const uint8 Count)
{
	// Todo. 데이터테이블 키인지 검증해야 함
	// IsValidDataTableKey<FItemInfo>(ItemKey);
	{
		if (ItemKey.IsValid() == false)
		{
			UE_LOG(LogPN, Warning, TEXT("AddItem ItemKey(%s)가 없습니다."), *ItemKey.ToString());
			return;
		}
	}

	if (Count <= 0)
	{
		UE_LOG(LogPN, Warning, TEXT("AddItem Count(%d)가 0보다 커야 합니다."), Count);
		return;
	}

	APawn* Player = GetPlayerController()->GetPawn();
	check(Player);

	UPNInventoryComponent* Inventory = Player->FindComponentByClass<UPNInventoryComponent>();
	check(Inventory);

	Inventory->RequestAddItem(ItemKey, Count);
}