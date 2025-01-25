// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PNAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
private:
	virtual void StartInitialLoading() override final;
};
