// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PNCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	bool IsIdle() const;

private:
	virtual void BeginPlay() override final;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override final;
	
	void OnUpdateTag(const FGameplayTag GameplayTag, int32 Count) const;
	
private:
	FDelegateHandle OnActionTagDelegateHandle;
};
