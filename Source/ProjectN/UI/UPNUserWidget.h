// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UPNUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UUPNUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void ToggleVisibility();
};
