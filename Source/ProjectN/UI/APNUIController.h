// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPNUserWidget.h"
#include "APNUIController.generated.h"

UCLASS()
class PROJECTN_API UAPNUIController : public UUPNUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
};
