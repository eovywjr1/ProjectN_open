// Fill out your copyright notice in the Description page of Project Settings.


#include "PNTargetMarkerUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UPNTargetMarkerUserWidget::SetTarget(FObjectKey InLockOnTargetObjectKey)
{
	LockOnTargetObjectKey = InLockOnTargetObjectKey;

	SetVisibility(ESlateVisibility::Visible);
}

void UPNTargetMarkerUserWidget::DeactivateMarker()
{
	LockOnTargetObjectKey = FObjectKey();

	SetVisibility(ESlateVisibility::Hidden);
}

void UPNTargetMarkerUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UPNTargetMarkerUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	AActor* LockOnTargetObject = Cast<AActor>(LockOnTargetObjectKey.ResolveObjectPtr());
	if (IsValid(LockOnTargetObject))
	{
		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			FVector2D ScreenPosition;
			if (PlayerController->ProjectWorldLocationToScreen(LockOnTargetObject->GetActorLocation(), ScreenPosition))
			{
				if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
				{
					ScreenPosition /= UWidgetLayoutLibrary::GetViewportScale(GetWorld());
					CanvasSlot->SetPosition(ScreenPosition);
				}
			}
		}
	}
}
