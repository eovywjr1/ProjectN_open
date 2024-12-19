// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "PNCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec)
	void AddItem(FName ItemKey, const uint8 Count = 1);
};
