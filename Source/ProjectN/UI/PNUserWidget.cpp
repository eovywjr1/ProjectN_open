// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PNUserWidget.h"

void UPNUserWidget::ToggleVisibility()
{
	const ESlateVisibility NewVisibility = GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	SetVisibility(NewVisibility);
}
