// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterPlayer.h"

#include "Component/PNEnhancedInputComponent.h"
#include "Component/PNPawnComponent.h"
#include "Component/PNPawnData.h"
#include "Component/PNPlayerInputComponent.h"
#include "AbilitySystem/PNAbilitySet.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "Component/PNEquipmentComponent.h"
#include "Component/PNInventoryComponent.h"
#include "Player/PNPlayerState.h"

APNCharacterPlayer::APNCharacterPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OverrideInputComponentClass = UPNEnhancedInputComponent::StaticClass();

	PNPlayerInputComponent = CreateDefaultSubobject<UPNPlayerInputComponent>(TEXT("PlayerInputComponent"));
	CreateDefaultSubobject<UPNInventoryComponent>(TEXT("InventoryComponent"));
	CreateDefaultSubobject<UPNEquipmentComponent>(TEXT("EquipmentComponent"));
}

void APNCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Todo. 캐릭터 스폰할 때 PawnData, AbilitySystemComponent 초기화해줘야 함
	if (APNPlayerState* PNPlayerState = GetPlayerState<APNPlayerState>())
	{
		UAbilitySystemComponent* ASComponent = PNPlayerState->GetAbilitySystemComponent();
		PawnComponent->SetAbilitySystemComponent(CastChecked<UPNAbilitySystemComponent>(ASComponent));
		ASComponent->InitAbilityActorInfo(PNPlayerState, this);

		if (const UPNPawnData* PawnData = PawnComponent->GetPawnData())
		{
			for (const UPNAbilitySet* AbilitySet : PawnData->AbilitySets)
			{
				if (AbilitySet)
				{
					AbilitySet->GiveAbilityToAbilitySystem(ASComponent, this);
				}
			}
		}
	}
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

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}
