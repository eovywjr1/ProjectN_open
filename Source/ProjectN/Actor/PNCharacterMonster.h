// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PNCharacter.h"
#include "PNCharacterMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API APNCharacterMonster : public APNCharacter
{
	GENERATED_BODY()
	
private:
	APNCharacterMonster();
	
	virtual void PreInitializeComponents() override final;
};
