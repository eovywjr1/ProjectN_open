// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void OnDetectedInteractableActor(const FObjectKey InteractionActorKey, const FName InteractionDataTableKey);
	void OnUnDetectedInteractableActor();
	
private:
	virtual void NativeConstruct() override final;
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override final;
	virtual FReply NativeOnKeyUp( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override final;
	
	void RequestInteraction() const;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractionText;
	
	float InteractionKeyRequirePressTime = 0.0f;
	
	FName CurrentInteractionKey;
	FObjectKey CurrentInteractionActorKey;
	
	FTimerHandle KeyPressTimerHandle;
};
