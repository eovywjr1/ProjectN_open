// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PNActorExtensionComponent.generated.h"

UENUM()
enum class EActorType : uint8
{
	NPC,
	Monster,
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
	UPNAbilitySystemComponent* GetAbilitySystemComponent() const;
	void InitializeAbilitySystem(UPNAbilitySystemComponent* InAbilitySystemComponent, AActor* InOwnerActor);
	
	bool IsPlayerActor() const { return ActorType == EActorType::Player; }
	bool IsMonsterActor() const { return ActorType == EActorType::Monster; }
	
private:
	UPNActorExtensionComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitializeComponent() override final;
	
public:
	EActorType ActorType = EActorType::MAX;
	
private:
	UPROPERTY()
	TObjectPtr<const UPNActorGameData> ActorGameData = nullptr;
	
	mutable TObjectPtr<UPNAbilitySystemComponent> AbilitySystemComponent = nullptr;
};
