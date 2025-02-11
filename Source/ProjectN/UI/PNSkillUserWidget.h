// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "PNSkillUserWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNSkillUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override final;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SkillImage;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag SkillInputTag;
};
