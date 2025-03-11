// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Component/PNActorComponentCreatorInterface.h"
#include "GameFramework/Character.h"
#include "Interface/PNAbilitySystemInterface.h"
#include "PNCharacter.generated.h"

class UPNActorExtensionComponent;

UCLASS(config=Game)
class APNCharacter : public ACharacter, public IPNAbilitySystemInterface, public IPNActorComponentCreatorInterface
{
	GENERATED_BODY()

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override final;

	bool IsPlayer() const;
	bool IsRun() const;

	void SetMaxWalkSpeed(const float InMaxSpeed);
	float GetMaxWalkSpeed() const;
	
	UFUNCTION(Server, Reliable)
	void ServerSetMaxWalkSpeed(const float InMaxSpeed);

	void OnInitializedStatus() const;

	virtual void SetDead();

protected:
	APNCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
private:
	virtual void PostInitializeComponents() override final;

protected:
	UPROPERTY()
	mutable TObjectPtr<UPNActorExtensionComponent> ActorExtensionComponent;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> DeadMontage;
};
