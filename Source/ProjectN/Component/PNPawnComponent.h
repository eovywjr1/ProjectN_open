// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"

#include "PNPawnComponent.generated.h"

/**
 * 
 */

class UPNAbilitySystemComponent;
class UPNPawnData;
 
UCLASS()
class PROJECTN_API UPNPawnComponent : public UPawnComponent
{
	GENERATED_BODY()
	
private:
	UPNPawnComponent(const FObjectInitializer& ObjectInitializer);
	
public:
	const UPNPawnData* GetPawnData() const { return PawnData; }
	UPNAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	void SetAbilitySystemComponent(UPNAbilitySystemComponent* InAbilitySystemComponent) { AbilitySystemComponent = InAbilitySystemComponent; }
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	TObjectPtr<const UPNPawnData> PawnData;

	UPROPERTY()
	TObjectPtr<UPNAbilitySystemComponent> AbilitySystemComponent;
};
