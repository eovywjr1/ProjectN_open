// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCharacter.h"
#include "PNCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API APNCharacterNonPlayer : public APNCharacter
{
	GENERATED_BODY()
	
private:
	APNCharacterNonPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual void PossessedBy(AController* NewController) override;
};
