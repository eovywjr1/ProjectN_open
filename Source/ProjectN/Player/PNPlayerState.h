// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/PNAbilitySystemInterface.h"
#include "PNPlayerState.generated.h"

class UPNAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class PROJECTN_API APNPlayerState : public APlayerState, public IPNAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	
private:
	APNPlayerState();
	
private:
	UPROPERTY()
	TObjectPtr<UPNAbilitySystemComponent> AbilitySystemComponent;
};
