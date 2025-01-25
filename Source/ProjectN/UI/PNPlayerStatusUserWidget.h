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
 
// Todo. 추후 UPNPlayerStatusUserWidget->UPNStatusUserWidget으로 변경해야 함
UCLASS()
class PROJECTN_API UPNPlayerStatusUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetTargetObject(FObjectKey InTargetObjectKey) { TargetObjectKey = InTargetObjectKey; }

private:
	virtual void NativeOnInitialized() override final;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override final;

private:
	UPROPERTY(EditAnywhere)
	EStatusType StatusType;

	UPROPERTY()
	TObjectPtr<UProgressBar> ProgressBar;

	FObjectKey TargetObjectKey;
};
