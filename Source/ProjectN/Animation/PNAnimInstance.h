// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PNAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPNAnimInstance();

	virtual void NativeBeginPlay() override final;
	
	UFUNCTION()
	void OnPeaceTagChanged(const FGameplayTag Tag, int32 NewCount);

private:
	UPROPERTY(BlueprintReadOnly, Category = "States", meta = (AllowPrivateAccess = "true"))
	bool bIsPeaceState;
};
