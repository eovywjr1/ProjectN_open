// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PNCharacterPlayer.h"

#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "Component/PNEnhancedInputComponent.h"
#include "Component/PNPlayerInputComponent.h"
#include "Component/PNEquipmentComponent.h"
#include "Component/PNInventoryComponent.h"
#include "Component/PNPawnComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PNPlayerState.h"

APNCharacterPlayer::APNCharacterPlayer()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	OverrideInputComponentClass = UPNEnhancedInputComponent::StaticClass();

	CreateDefaultSubobject<UPNEquipmentComponent>(TEXT("EquipmentComponent"));
	PawnComponent->ActorType = EActorType::Player;

	PrimaryActorTick.bCanEverTick = true;
}

void APNCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!IsRun())
	{
		RunTargetRotationYaw = 0.0f;
	}
	
	FRotator TargetRotation = GetActorRotation();
	TargetRotation.Yaw = Controller->GetControlRotation().Yaw;

	if (RunTargetRotationYaw != 0.0f)
	{
		TargetRotation.Yaw += RunTargetRotationYaw;
	}
	
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, 10.0f);
	SetActorRotation(NewRotation);
}

void APNCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitialComponents();
}

void APNCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitialComponents();
}

void APNCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PNPlayerInputComponent)
	{
		PNPlayerInputComponent->InitializePlayerInput(InputComponent);
	}
}

void APNCharacterPlayer::InitialComponents()
{
	APNPlayerState* PlayerStateCast = GetPlayerState<APNPlayerState>();
	check(PlayerStateCast);

	UAbilitySystemComponent* AbilitySystemComponent = PlayerStateCast->GetAbilitySystemComponent();
	PawnComponent->InitializeAbilitySystem(Cast<UPNAbilitySystemComponent>(AbilitySystemComponent), PlayerStateCast);

	if (IsLocallyControlled())
	{
		PNPlayerInputComponent = NewObject<UPNPlayerInputComponent>(this, TEXT("PlayerInputComponent"));
		PNPlayerInputComponent->RegisterComponent();

		// PlayerState 리플리케이션이 늦을 경우 InputComponent가 먼저 생생하는 상황 대비하여 초기화
		// 보통은 PlayerInputComponent가 먼저 생성되므로 InputComponent가 nullptr
		PNPlayerInputComponent->InitializePlayerInput(InputComponent);

		UPNInventoryComponent* InventoryComponent = NewObject<UPNInventoryComponent>(this, TEXT("InventoryComponent"));
		InventoryComponent->RegisterComponent();
	}
}

void APNCharacterPlayer::MoveByInput(const FVector2D MovementVector)
{
	check(Controller);

	if (IsRun() && MovementVector.Y >= 0.0f && FMath::Abs(MovementVector.X) > 0.0f)
	{
		RunTargetRotationYaw = FMath::RadiansToDegrees(FMath::Atan2(MovementVector.X, MovementVector.Y));
	}
	else
	{
		RunTargetRotationYaw = 0.0f;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APNCharacterPlayer::SetDead()
{
	Super::SetDead();

	DisableInput(Cast<APlayerController>(GetController()));
}
