// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPNUserWidget.h"

void UUPNUserWidget::ToggleVisibility()
{
	const ESlateVisibility NewVisibility = GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	SetVisibility(NewVisibility);
}
