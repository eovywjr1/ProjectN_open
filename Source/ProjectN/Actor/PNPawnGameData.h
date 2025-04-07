// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCommonModule.h"
#include "Actor/PNActorGameData.h"
#include "PNPawnGameData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNPawnGameData : public UPNActorGameData
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const TArray<FComboData>* GetComboDatas() const { return &ComboDatas; }
	
private:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override { return FPrimaryAssetId("PawnGameData", GetFName()); };
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FComboData> ComboDatas;
};
