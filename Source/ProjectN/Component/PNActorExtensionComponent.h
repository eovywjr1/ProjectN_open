// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PNActorExtensionComponent.generated.h"

UENUM()
enum class EActorType : uint8
{
	NPC,
	Enemy,
	Player,
	MAX
};

/**
 * 
 */

class UPNAbilitySystemComponent;
class UPNActorGameData;
 
UCLASS()
class PROJECTN_API UPNActorExtensionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPNAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	void InitializeAbilitySystem(UPNAbilitySystemComponent* InAbilitySystemComponent, AActor* InOwnerActor);
	
private:
	UPNActorExtensionComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitializeComponent() override final; 
	virtual void BeginPlay() override final;
	
public:
	EActorType ActorType = EActorType::MAX;
	
private:
	UPROPERTY()
	TObjectPtr<const UPNActorGameData> ActorGameData = nullptr;
	
	UPROPERTY()
	TObjectPtr<UPNAbilitySystemComponent> AbilitySystemComponent = nullptr;
};
