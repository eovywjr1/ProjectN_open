// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PNUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNUserWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class APNHUD;
	
protected:
	UFUNCTION(BlueprintCallable)
	void ToggleVisibility();
};
