// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCharacter.h"
#include "PNCharacterPlayer.generated.h"

class UPNPlayerInputComponent;

/**
 * 
 */
UCLASS()
class PROJECTN_API APNCharacterPlayer : public APNCharacter
{
	GENERATED_BODY()

public:
	void MoveByInput(const FVector2D MovementVector);
	
	virtual void SetDead() override final;

private:
	APNCharacterPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override final;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPNPlayerInputComponent> PNPlayerInputComponent;
};
