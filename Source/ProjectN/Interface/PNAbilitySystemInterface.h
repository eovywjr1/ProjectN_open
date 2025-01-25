// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "UObject/Interface.h"
#include "PNAbilitySystemInterface.generated.h"

class UPNAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE(FOnInitializeAbilitySystemDelegate)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPNAbilitySystemInterface : public UAbilitySystemInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTN_API IPNAbilitySystemInterface : public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	FOnInitializeAbilitySystemDelegate OnInitializeAbilitySystemDelegate;
};
