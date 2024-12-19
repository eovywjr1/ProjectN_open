// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNInventoryComponent.h"

#include "Subsystem/PNGameDataSubsystem.h"

// Todo. 최대슬롯 20은 임시, 추후 정해야 함
constexpr uint8 InventorySlotCount = 20;

FPNInventorySlot::FPNInventorySlot(const FName InItemKey, uint8 InStackCount)
	: ItemKey(InItemKey),
	  StackCount(InStackCount)
{}

void UPNInventoryComponent::RequestAddItem(const FName ItemKey, const uint8 Count)
{
	// Todo. 데이터테이블 키인지 검증해야 함
	// IsValidDataTableKey<FItemInfo>(ItemKey);
	{
		if (ItemKey.IsValid() == false)
		{
			return;
		}
	}

	if (Count <= 0)
	{
		return;
	}

	if (Inventory.Num() == Inventory.Max())
	{
		return;
	}

	FPNInventorySlot* Slot = Inventory.FindByPredicate([ItemKey](const FPNInventorySlot& Slot)
	{
		return Slot.GetItemKey() == ItemKey;
	});
	if (Slot)
	{
		Slot->AddStackCount(Count);
		return;
	}

	Inventory.Emplace(ItemKey, Count);

	// Todo. UI Update
}

void UPNInventoryComponent::RequestRemoveItem(const FName ItemKey, const uint8 Count)
{
	// Todo. 데이터테이블 키인지 검증해야 함
	// IsValidDataTableKey<FItemInfo>(ItemKey);
	{
		if (ItemKey.IsValid() == false)
		{
			return;
		}
	}

	if (Count <= 0)
	{
		return;
	}

	FPNInventorySlot* Slot = Inventory.FindByPredicate([ItemKey](const FPNInventorySlot& Slot)
	{
		return Slot.GetItemKey() == ItemKey;
	});
	if (Slot == nullptr)
	{
		return;
	}

	RemoveItem(Slot, Count);

	// Todo. 만약 아이템을 버렸을 때 드랍된다면 로직 추가

	// Todo. UI Update
}

void UPNInventoryComponent::RequestUseItem(const FName ItemKey, const uint8 Count)
{
	// Todo. 데이터테이블 키인지 검증해야 함
	// IsValidDataTableKey<FItemInfo>(ItemKey);
	{
		if (ItemKey.IsValid() == false)
		{
			return;
		}
	}

	if (Count <= 0)
	{
		return;
	}

	FPNInventorySlot* Slot = Inventory.FindByPredicate([ItemKey](const FPNInventorySlot& Slot)
	{
		return Slot.GetItemKey() == ItemKey;
	});
	if (Slot == nullptr)
	{
		return;
	}

	const uint8 UseItemCount = RemoveItem(Slot, Count);

	// Todo. 아이템 사용 요청

	// Todo. UI Update
}

UPNInventoryComponent::UPNInventoryComponent()
{
	Inventory.Reserve(InventorySlotCount);
}

uint8 UPNInventoryComponent::RemoveItem(FPNInventorySlot* ItemSlot, const uint8 Count)
{
	if (Count <= 0)
	{
		return 0;
	}

	const uint8 RemoveStackCount = FMath::Min(ItemSlot->GetStackCount(), Count);

	ItemSlot->SubtractionStackCount(Count);

	if (ItemSlot->GetStackCount() == 0)
	{
		Inventory.Remove(*ItemSlot);
	}
	
	return RemoveStackCount;
}
