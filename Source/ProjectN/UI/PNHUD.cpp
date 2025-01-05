// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PNHUD.h"

#include "Blueprint/UserWidget.h"

void APNHUD::BeginPlay()
{
	Super::BeginPlay();

	for (TPair<EUIControllerID, FUIControllerData>& Controller : Controllers)
	{
		if (Controller.Value.ControllerClass)
		{
			Controller.Value.Controller = CreateWidget<UUserWidget>(GetWorld(), Controller.Value.ControllerClass);
			if (Controller.Value.Controller)
			{
				Controller.Value.Controller->AddToViewport(Controller.Value.Layer);
			}
		}
	}
}
