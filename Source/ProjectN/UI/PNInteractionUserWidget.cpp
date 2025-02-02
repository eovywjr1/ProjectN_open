// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PNInteractionUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Component/PNInteractionComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "DataTable/InteractionDataTable.h"
#include "Subsystem/PNGameDataSubsystem.h"

void UPNInteractionUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

	SetVisibility(ESlateVisibility::Hidden);
}

FReply UPNInteractionUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::F)
	{
		if (InteractionKeyRequirePressTime == 0.0f)
		{
			RequestInteraction();
		}
		else if (!GetWorld()->GetTimerManager().IsTimerActive(KeyPressTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(KeyPressTimerHandle, this, &UPNInteractionUserWidget::RequestInteraction, InteractionKeyRequirePressTime, false);
		}
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UPNInteractionUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::F)
	{
		GetWorld()->GetTimerManager().ClearTimer(KeyPressTimerHandle);
	}

	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UPNInteractionUserWidget::RequestInteraction() const
{
	UPNInteractionComponent* InteractionComponent = GetOwningPlayerPawn()->FindComponentByClass<UPNInteractionComponent>();
	InteractionComponent->RequestInteraction(CurrentInteractionActorKey, CurrentInteractionKey);
}

void UPNInteractionUserWidget::OnDetectedInteractableActor(const FObjectKey InteractionActorKey, const FName InteractionDataTableKey)
{
	AActor* InteractableActor = Cast<AActor>(InteractionActorKey.ResolveObjectPtr());
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

	if (CurrentInteractionActorKey != InteractionActorKey && CurrentInteractionKey != InteractionDataTableKey)
	{
		if (const FInteractionDataTable* InteractionDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FInteractionDataTable>(InteractionDataTableKey))
		{
			InteractionText->SetText(FText::FromName(InteractionDataTable->GetUIText()));
			InteractionKeyRequirePressTime = InteractionDataTable->GetKeyPressTime();
		}
	}

	CurrentInteractionKey = InteractionDataTableKey;
	CurrentInteractionActorKey = InteractionActorKey;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	check(CanvasSlot);

	ScreenPosition /= UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	CanvasSlot->SetPosition(ScreenPosition);
	SetVisibility(ESlateVisibility::Visible);
	SetFocus();
}

void UPNInteractionUserWidget::OnUnDetectedInteractableActor()
{
	SetVisibility(ESlateVisibility::Hidden);

	CurrentInteractionKey = NAME_None;
	CurrentInteractionActorKey = FObjectKey();
}
