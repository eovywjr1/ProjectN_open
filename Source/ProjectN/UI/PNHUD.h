// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PNHUD.generated.h"

enum class EStatusType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(GameEvent_OnStatusChangedDelegate, FObjectKey, EStatusType);
DECLARE_MULTICAST_DELEGATE_OneParam(GameEvent_OnInitStatusDelegate, FObjectKey);

/**
 * 
 */
UCLASS()
class PROJECTN_API APNHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	GameEvent_OnStatusChangedDelegate OnStatusChangedDelegate;
	GameEvent_OnInitStatusDelegate OnInitStatusDelegate;
	
private:
	virtual void BeginPlay() override final;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY()
	UUserWidget* HUDWidget;
};
