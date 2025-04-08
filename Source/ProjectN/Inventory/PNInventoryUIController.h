// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/APNUIController.h"
#include "PNInventoryUIController.generated.h"

class UPNInventorySlotUserWidget;

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNInventoryUIController : public UAPNUIController
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UPNUserWidget* AddInventorySlotUserWidget();
	
private:
	virtual void NativeConstruct() override final;

	UFUNCTION()
	void OnUpdateInventory();

private:
	TMap<FName, TObjectPtr<UPNUserWidget>> InventorySlotWidgets;
};
