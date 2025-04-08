// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PNInventorySlotUserWidget.h"

void UPNInventorySlotUserWidget::UpdateSlot(const FName InItemKey, const uint8 InStackCount)
{
	ItemKey = InItemKey;
	StackCount = InStackCount;
	
	// Image update
}
