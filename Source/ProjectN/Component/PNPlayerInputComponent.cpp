// Fill out your copyright notice in the Description page of Project Settings.


#include "PNPlayerInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "PNEnhancedInputComponent.h"
#include "PNGameplayTags.h"
#include "PNPawnComponent.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "Actor/PNCharacterPlayer.h"
#include "Input/PNInputConfig.h"
#include "InputMappingContext.h"
#include "Engine/AssetManager.h"
#include "Player/PNPlayerController.h"

UPNPlayerInputComponent::UPNPlayerInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void UPNPlayerInputComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	if (PlayerInputComponent == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController->IsLocalController())
	{
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())->AddMappingContext(CameraMappingContext, 0);
		EnableControlInput(true);
	}

	const FPNGameplayTags& GameplayTags = FPNGameplayTags::Get();
	UPNEnhancedInputComponent* PNEnhancedInputComponent = Cast<UPNEnhancedInputComponent>(PlayerInputComponent);

	PNEnhancedInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	PNEnhancedInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	PNEnhancedInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_LockOn, ETriggerEvent::Triggered, this, &ThisClass::Input_LockOn);
	PNEnhancedInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_NextLockOnTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_NextLockOnTarget);

	PNEnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityPressed, &ThisClass::Input_AbilityReleased);
}

void UPNPlayerInputComponent::EnableControlInput(bool bEnable) const
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	if (!PlayerController->IsLocalController())
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (bEnable)
	{
		Subsystem->AddMappingContext(ControlMappingContext, 0);
	}
	else
	{
		Subsystem->RemoveMappingContext(ControlMappingContext);
	}
}

void UPNPlayerInputComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	const UAssetManager& AssetManager = UAssetManager::Get();
	
	{
		FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(FPrimaryAssetId(FName(TEXT("PlayerInput")), FName(TEXT("IMC_Control")))));
		if(AssetPtr.IsPending())
		{
			AssetPtr.LoadSynchronous();
		}
		
		ControlMappingContext = Cast<UInputMappingContext>(AssetPtr.Get());
		check(ControlMappingContext);
	}
	
	{
		FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(FPrimaryAssetId(FName(TEXT("PlayerInput")), FName(TEXT("IMC_Camera")))));
		if(AssetPtr.IsPending())
		{
			AssetPtr.LoadSynchronous();
		}
		
		CameraMappingContext = Cast<UInputMappingContext>(AssetPtr.Get());
		check(CameraMappingContext);
	}
	
	{
		FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(FPrimaryAssetId(FName(TEXT("PlayerInput")), FName(TEXT("DA_PlayerInputConfig")))));
		if(AssetPtr.IsPending())
		{
			AssetPtr.LoadSynchronous();
		}
		
		InputConfig = Cast<UPNInputConfig>(AssetPtr.Get());
		check(InputConfig);
	}
}

void UPNPlayerInputComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APNCharacterPlayer* Owner = GetPawn<APNCharacterPlayer>();
	check(Owner);

	Owner->MoveByInput(InputActionValue.Get<FVector2D>());

	LastMovementInput = InputActionValue.Get<FVector2D>();
}

void UPNPlayerInputComponent::Input_Look(const FInputActionValue& InputActionValue)
{
	GetController<APNPlayerController>()->RotationByInput(InputActionValue.Get<FVector2D>());
}

void UPNPlayerInputComponent::Input_NextLockOnTarget(const FInputActionValue& InputActionValue)
{
	if (bIsActivatedLockOn == false)
	{
		return;
	}

	GetController<APNPlayerController>()->SetNextPriorityLockOnTargetActor();
}

void UPNPlayerInputComponent::Input_LockOn(const FInputActionValue& InputActionValue)
{
	bIsActivatedLockOn = !bIsActivatedLockOn;

	if (bIsActivatedLockOn)
	{
		GetController<APNPlayerController>()->ActivateLockOn();
	}
	else
	{
		GetController<APNPlayerController>()->DeActivateLockOn();
	}
}

void UPNPlayerInputComponent::Input_AbilityPressed(FGameplayTag InputTag)
{
	APawn* Owner = GetPawn<APawn>();
	if (Owner == nullptr)
	{
		return;
	}

	const UPNPawnComponent* PawnComponent = Owner->FindComponentByClass<UPNPawnComponent>();
	if (PawnComponent == nullptr)
	{
		return;
	}

	UPNAbilitySystemComponent* ASC = PawnComponent->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return;
	}

	ASC->AbilityInputPressed(InputTag);
}

void UPNPlayerInputComponent::Input_AbilityReleased(FGameplayTag InputTag)
{
	APawn* Owner = GetPawn<APawn>();
	if (Owner == nullptr)
	{
		return;
	}

	const UPNPawnComponent* PawnComponent = Owner->FindComponentByClass<UPNPawnComponent>();
	if (PawnComponent == nullptr)
	{
		return;
	}

	UPNAbilitySystemComponent* ASC = PawnComponent->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return;
	}

	ASC->AbilityInputReleased(InputTag);
}
