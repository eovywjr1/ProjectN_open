// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APNUIController.h"

void UAPNUIController::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetVisibility(ESlateVisibility::Hidden);
}
