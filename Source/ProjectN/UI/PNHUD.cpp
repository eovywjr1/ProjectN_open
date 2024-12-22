// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PNHUD.h"

#include "Blueprint/UserWidget.h"

void APNHUD::BeginPlay()
{
	Super::BeginPlay();
	
	// Todo. 임시 추후 월드 진입할 때로 변경
	HUDWidget = CreateWidget(GetOwningPlayerController(), HUDClass, FName(TEXT("HUDWidget")));
	HUDWidget->AddToViewport();
}
