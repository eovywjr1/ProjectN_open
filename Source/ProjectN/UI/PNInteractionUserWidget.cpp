// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PNInteractionUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UPNInteractionUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UPNInteractionUserWidget::OnDetectedInteractableActor(FInteractionOption InteractionOption)
{
	AActor* InteractableActor = Cast<AActor>(InteractionOption.InteractionTargetActorKey.ResolveObjectPtr());
	if (!IsValid(InteractableActor))
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayer();
	FVector2D ScreenPosition;
	if (!PlayerController->ProjectWorldLocationToScreen(InteractableActor->GetActorLocation(), ScreenPosition))
	{
		return;
	}

	int32 ViewportSizeX = 0, ViewportSizeY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	if (ScreenPosition.X < 0 || ScreenPosition.X > ViewportSizeX || ScreenPosition.Y < 0 || ScreenPosition.Y > ViewportSizeY)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	// Todo. InteractionDataTable에 대해서 모두 구현 완료 후 구현
	// InteractionText->SetText(FText::FromName(InteractionOption.InteractionUIText));
	// InteractionKeyRequirePressTime = InteractionOption.InteractionKeyPressTime;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	check(CanvasSlot);
	
	ScreenPosition /= UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	CanvasSlot->SetPosition(ScreenPosition);
	SetVisibility(ESlateVisibility::Visible);
}

void UPNInteractionUserWidget::OnUnDetectedInteractableActor()
{
	SetVisibility(ESlateVisibility::Hidden);
}
