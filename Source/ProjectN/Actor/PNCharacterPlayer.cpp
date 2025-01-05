// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterPlayer.h"

#include "Component/PNEnhancedInputComponent.h"
#include "Component/PNPlayerInputComponent.h"
#include "Component/PNEquipmentComponent.h"
#include "Component/PNInventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APNCharacterPlayer::APNCharacterPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseControllerRotationYaw = true;

	OverrideInputComponentClass = UPNEnhancedInputComponent::StaticClass();

	PNPlayerInputComponent = CreateDefaultSubobject<UPNPlayerInputComponent>(TEXT("PlayerInputComponent"));
	CreateDefaultSubobject<UPNInventoryComponent>(TEXT("InventoryComponent"));
	CreateDefaultSubobject<UPNEquipmentComponent>(TEXT("EquipmentComponent"));
}

void APNCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PNPlayerInputComponent);
	PNPlayerInputComponent->InitializePlayerInput(PlayerInputComponent);
}

void APNCharacterPlayer::MoveByInput(const FVector2D MovementVector)
{
	check(Controller);
	
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	if (IsRun() && MovementVector.Y >= 0.0f && FMath::Abs(MovementVector.X) > 0.0f)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}
