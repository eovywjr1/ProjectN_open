// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PNUserWidget.h"
#include "PNInventorySlotUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNInventorySlotUserWidget : public UPNUserWidget
{
	GENERATED_BODY()

public:
	void UpdateSlot(const FName InItemKey, const uint8 InStackCount);

protected:
	UPROPERTY()
	FName ItemKey;

	UPROPERTY(BlueprintReadOnly)
	uint8 StackCount = 0;
};
