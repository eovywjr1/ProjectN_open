// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PNHUDUIController.h"

#include "PNHUD.h"
#include "PNInteractionUserWidget.h"
#include "PNPlayerStatusUserWidget.h"
#include "PNTargetMarkerUserWidget.h"

void UPNHUDUIController::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	APNHUD* HUD = Cast<APNHUD>(GetOwningPlayer()->GetHUD());
	check(HUD);

	HUD->OnSetLockOnTargetDelegate.AddUObject(this, &ThisClass::SetLockOnTarget);
	HUD->OnDeactivatedLockOnDelegate.AddUObject(this, &ThisClass::OnDeactivatedLockOn);
	
	HUD->OnDetectedInteractableActorDelegate.AddUObject(InteractionWidget, &UPNInteractionUserWidget::OnDetectedInteractableActor);
	HUD->OnUnDetectedInteractableActorDelegate.AddUObject(InteractionWidget, &UPNInteractionUserWidget::OnUnDetectedInteractableActor);
}

void UPNHUDUIController::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Todo. 임시 추후 월드 진입할 때로 변경
	SetVisibility(ESlateVisibility::Visible);
	
	APawn* PlayerPawn = GetOwningPlayerPawn();
	
	PlayerHpWidget->SetTargetObject(PlayerPawn);
	PlayerSRWidget->SetTargetObject(PlayerPawn);
}

void UPNHUDUIController::SetLockOnTarget(FObjectKey LockOnTargetObjectKey)
{
	LockOnTargetMarkerWidget->SetTarget(LockOnTargetObjectKey);
}

void UPNHUDUIController::OnDeactivatedLockOn()
{
	LockOnTargetMarkerWidget->DeactivateMarker();
}
