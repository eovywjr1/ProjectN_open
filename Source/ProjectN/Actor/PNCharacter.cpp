// Copyright Epic Games, Inc. All Rights Reserved.

#include "PNCharacter.h"

#include "PNGameplayTags.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PNPawnAttributeSet.h"
#include "Component/PNCharacterMovementComponent.h"
#include "Component/PNDetectComponent.h"
#include "Component/PNActorExtensionComponent.h"
#include "Component/PNSkillComponent.h"
#include "Component/PNStatusActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"

//////////////////////////////////////////////////////////////////////////

void APNCharacter::SetMaxWalkSpeed(const float InMaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InMaxSpeed;

	if (!HasAuthority())
	{
		ServerSetMaxWalkSpeed(InMaxSpeed);
	}
}

float APNCharacter::GetMaxWalkSpeed() const
{
	return GetCharacterMovement()->MaxWalkSpeed;
}

void APNCharacter::ServerSetMaxWalkSpeed_Implementation(const float InMaxSpeed)
{
	SetMaxWalkSpeed(InMaxSpeed);
}

void APNCharacter::OnInitializedStatus() const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = AbilitySystemComponent->GetSet<UPNPawnAttributeSet>()->GetWalkSpeed();
}

void APNCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	if (DeadMontage)
	{
		AnimInstance->Montage_Play(DeadMontage, 1.0f);
	}
}

APNCharacter::APNCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPNCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
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
}

void APNCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ActorExtensionComponent = FindComponentByClass<UPNActorExtensionComponent>();
	check(ActorExtensionComponent);
}

UAbilitySystemComponent* APNCharacter::GetAbilitySystemComponent() const
{
	return ActorExtensionComponent->GetAbilitySystemComponent();
}

bool APNCharacter::IsPlayer() const
{
	if (Controller == nullptr)
	{
		return false;
	}

	return Controller->IsPlayerController();
}

bool APNCharacter::IsRun() const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
	{
		return false;
	}

	return AbilitySystemComponent->HasMatchingGameplayTag(FPNGameplayTags::Get().Action_Run);
}
