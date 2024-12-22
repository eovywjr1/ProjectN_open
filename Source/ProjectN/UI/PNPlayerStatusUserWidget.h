// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PNPlayerStatusUserWidget.generated.h"

class UTextBlock;
class UProgressBar;
class APNCharacter;

enum class EStatusType : uint8;

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNPlayerStatusUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeOnInitialized() override final;
	
	void OnStatusChanged(const FObjectKey TargetObjectKey, const EStatusType InStatusType);
	void OnInitStatus(const FObjectKey TargetObjectKey);
	void UpdateStatus(const FObjectKey TargetObjectKey) const;
	
private:
	UPROPERTY(EditAnywhere)
	EStatusType StatusType;
	
	UPROPERTY()
	TObjectPtr<UProgressBar> ProgressBar;
};
