// Fill out your copyright notice in the Description page of Project Settings.


#include "PNInventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Subsystem/PNGameDataSubsystem.h"

FPNInventorySlot::FPNInventorySlot()
	: ItemKey(NAME_None),
	  StackCount(0)
{}

FPNInventorySlot::FPNInventorySlot(const FName InItemKey, uint8 InStackCount)
	: ItemKey(InItemKey),
	  StackCount(InStackCount)
{}

void UPNInventoryComponent::RequestAddItem(const FName ItemKey, const uint8 Count)
{
	if (!IsEnableAddItem(ItemKey, Count))
	{
		return;
	}

	ServerAddItem(ItemKey, Count);
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

	FPNInventorySlot* Slot = Slots.FindByPredicate([ItemKey](const FPNInventorySlot& Slot)
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
	FPNInventorySlot* Slot = Slots.FindByPredicate([ItemKey](const FPNInventorySlot& Slot)
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
	SetIsReplicatedByDefault(true);

	// Todo. 최대슬롯 20은 임시, 추후 정해야 함
	const uint8 InventorySlotCount = 20;
	Slots.Reserve(InventorySlotCount);
}

void UPNInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPNInventoryComponent, Slots);
}

void UPNInventoryComponent::ServerAddItem_Implementation(const FName ItemKey, const uint8 Count)
{
	if (!IsEnableAddItem(ItemKey, Count))
	{
		return;
	}
	
	FPNInventorySlot* Slot = Slots.FindByPredicate([ItemKey](const FPNInventorySlot& Slot)
	{
		return Slot.GetItemKey() == ItemKey;
	});
	if (Slot)
	{
		Slot->AddStackCount(Count);
		MARK_PROPERTY_DIRTY_FROM_NAME(UPNInventoryComponent, Slots, this);
	}
	else
	{
		Slots.Emplace(ItemKey, Count);
	}

	if (GetNetMode() == ENetMode::NM_Standalone)
	{
		OnRep_Slots();
	}
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
		Slots.Remove(*ItemSlot);
	}
	
	return RemoveStackCount;
}

void UPNInventoryComponent::OnRep_Slots()
{
	// Todo. UpdateUI
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("디버그 메시지"));
}

bool UPNInventoryComponent::IsValidItem(const FName ItemKey) const
{
	// Todo. 데이터테이블 키인지 검증해야 함
	// IsValidDataTableKey<FItemInfo>(ItemKey);
	{
		if (!ItemKey.IsValid())
		{
			return false;
		}
	}
	
	return true;
}

bool UPNInventoryComponent::IsEnableAddItem(const FName ItemKey, const uint8 Count) const
{
	if (!IsValidItem(ItemKey))
	{
		return false;
	}

	if (Count <= 0)
	{
		check(false);
		return false;
	}
	
	if (Slots.Num() == Slots.Max())
	{
		return false;
	}

	return true;
}
