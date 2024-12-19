// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PNStatusActorComponent.generated.h"

class UPNPawnAttributeSet;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNStatusActorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override final;
};
