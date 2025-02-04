// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PNCharacter.h"
#include "Component/PNActorComponentCreatorInterface.h"
#include "PNCharacterEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API APNCharacterEnemy : public APNCharacter, public IPNActorComponentCreatorInterface
{
	GENERATED_BODY()
	
private:
	virtual void PreInitializeComponents() override final;
};
