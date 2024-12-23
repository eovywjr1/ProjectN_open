// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"

#include "PNPlayerInputComponent.generated.h"

/**
 * 
 */

struct FGameplayTag;
struct FInputActionValue;

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNPlayerInputComponent : public UPawnComponent
{
	GENERATED_BODY()

private:
	UPNPlayerInputComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override final;
	
public:
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void EnableInput(bool bIsEnable) const;
	FVector2D GetLastMovementInput() const { return LastMovementInput; }

private:
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_EnableLockOn(const FInputActionValue& InputActionValue);
	void Input_LockOn(const FInputActionValue& InputActionValue);
	
	void Input_AbilityPressed(FGameplayTag InputTag);
	void Input_AbilityReleased(FGameplayTag InputTag);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> CameraMappingContext;
	
	FVector2D LastMovementInput;
	
	bool bIsEnableLockOn = true;
};
