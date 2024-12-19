// Copyright Epic Games, Inc. All Rights Reserved.

#include "PNCharacter.h"

#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "Component/PNPawnComponent.h"
#include "Component/PNStatusActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

//////////////////////////////////////////////////////////////////////////

APNCharacter::APNCharacter(const FObjectInitializer& ObjectInitializer)
	: ACharacter(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	/////////////////////////////////////////////////////////////////////////////////////////

	PawnComponent = CreateDefaultSubobject<UPNPawnComponent>(TEXT("PNPawnComponent"));
	CreateDefaultSubobject<UPNStatusActorComponent>(TEXT("StatusActorComponent"));
}

UAbilitySystemComponent* APNCharacter::GetAbilitySystemComponent() const
{
	return PawnComponent->GetAbilitySystemComponent();
}

bool APNCharacter::IsIdle() const
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	check(CharacterMovementComponent);

	if (const bool bIsMoving = CharacterMovementComponent->Velocity.IsNearlyZero() == false)
	{
		return false;
	}

	if (CharacterMovementComponent->IsFalling())
	{
		return false;
	}

	return true;
}
