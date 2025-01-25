// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PNPawnData.generated.h"

class UPNAbilitySet;

/**
 * 
 */
UCLASS(Const)
class PROJECTN_API UPNPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
private:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override { return FPrimaryAssetId("PawnData", GetFName()); };
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UPNAbilitySet>> AbilitySets;	
};
