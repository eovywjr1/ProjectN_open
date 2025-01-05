// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PNTargetMarkerUserWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNTargetMarkerUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetTarget(FObjectKey InLockOnTargetObjectKey);
	void DeactivateMarker();
	
private:
	virtual void NativeConstruct() override final;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override final;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MarkerImage;
	
	FObjectKey LockOnTargetObjectKey;
};
