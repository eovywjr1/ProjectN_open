// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCharacter.h"
#include "PNCharacterPlayer.generated.h"

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
	APNCharacterPlayer();
	
	virtual void PreInitializeComponents() override final;
	virtual void Tick(float DeltaSeconds) override final;
	virtual void PossessedBy(AController* NewController) override final;
	virtual void OnRep_PlayerState() override final;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void InitializeAbilitySystemComponent();
	
private:
	float RunTargetRotationYaw = 0.0f;
	
	bool bDisableMove = false;
};
