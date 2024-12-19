// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PNPawnData.generated.h"

class UPNInputConfig;
class UPNAbilitySet;

/**
 * 
 */
UCLASS(BlueprintType, Const)
class PROJECTN_API UPNPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPNInputConfig> InputConfig;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UPNAbilitySet>> AbilitySets;	
};
