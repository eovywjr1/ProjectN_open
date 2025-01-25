// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCommonModule.h"
#include "Blueprint/UserWidget.h"
#include "PNInteractionUserWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTN_API UPNInteractionUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void OnDetectedInteractableActor(FInteractionOption InteractionOption);
	void OnUnDetectedInteractableActor();
	
private:
	virtual void NativeConstruct() override final;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractionText;
	
	float InteractionKeyRequirePressTime = 0.0f;
	float InteractionKeyPressTime = 0.0f;
};
