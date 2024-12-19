// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PNAbilitySet.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FPNAbilitySet_GameplayAbility{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	void GiveAbilityToAbilitySystem(UAbilitySystemComponent* InAbilitySystemComponent, UObject* SourceObject) const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TArray<FPNAbilitySet_GameplayAbility> GameAbilities;
};
