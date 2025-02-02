// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNInteractionComponent.generated.h"

struct FObjectKey;
struct FGameplayTagContainer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UPNInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool CanInteraction() const;
	FName GetInteractionKey();
	
	void RequestInteraction(const FObjectKey InteractionTargetActorKey, const FName InteractionKey);
	
	UFUNCTION(Server, Reliable)
	void ServerProcessInteraction(AActor* InteractionTargetActor, const FName InteractionKey);
	
	void TryActivateInteractionAbility(const FGameplayTagContainer& InteractionTag) const;
	
private:
	UPNInteractionComponent();
};
