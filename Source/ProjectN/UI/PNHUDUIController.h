// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APNUIController.h"
#include "PNHUDUIController.generated.h"

class UPNPlayerStatusUserWidget;
class UPNInteractionUserWidget;
class UPNTargetMarkerUserWidget;

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNHUDUIController : public UAPNUIController
{
	GENERATED_BODY()
	
private:
	virtual void NativeOnInitialized() override final;
	virtual void NativeConstruct() override final;
	
	void SetLockOnTarget(FObjectKey LockOnTargetObjectKey);
	void OnDeactivatedLockOn();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPNTargetMarkerUserWidget> LockOnTargetMarkerWidget;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPNInteractionUserWidget> InteractionWidget;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPNPlayerStatusUserWidget> PlayerHpWidget;
};
