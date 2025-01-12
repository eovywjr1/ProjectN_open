// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "PNCharacter.generated.h"

class UPNPawnComponent;

UCLASS(config=Game)
class APNCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override final;

	bool IsPlayer() const;
	bool IsRun() const;

	void SetMaxWalkSpeed(const float InMaxSpeed);
	float GetMaxWalkSpeed() const;

	void OnInitializedStatus() const;

	virtual void SetDead();

protected:
	APNCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPNPawnComponent> PawnComponent;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> DeadMontage;
};
