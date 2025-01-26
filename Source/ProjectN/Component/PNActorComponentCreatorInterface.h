// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PNActorComponentCreatorInterface.generated.h"

enum class EActorType : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPNActorComponentCreatorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTN_API IPNActorComponentCreatorInterface
{
	GENERATED_BODY()

public:
	void CreateActorComponent(EActorType ActorType);
	
};
