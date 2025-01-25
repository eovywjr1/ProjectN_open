// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNCommonModule.h"
#include "PNInteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UPNInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool CanInteraction() const;
	bool GetInteractionOption(FInteractionOption& InInteractionOption);
	
private:
	FName InteractionDataTableKey;
};
