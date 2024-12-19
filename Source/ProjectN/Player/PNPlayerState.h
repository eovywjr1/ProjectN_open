// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PNPlayerState.generated.h"

class UPNAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class PROJECTN_API APNPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APNPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override final;

private:
	UPROPERTY()
	TObjectPtr<UPNAbilitySystemComponent> AbilitySystemComponent;
};
