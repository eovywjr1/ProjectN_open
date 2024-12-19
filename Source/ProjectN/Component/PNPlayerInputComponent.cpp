// Fill out your copyright notice in the Description page of Project Settings.


#include "PNPlayerInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagsManager.h"
#include "PNEnhancedInputComponent.h"
#include "PNGameplayTags.h"
#include "PNPawnComponent.h"
#include "PNPawnData.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "Actor/PNCharacterPlayer.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Input/PNInputConfig.h"
#include "Player/PNPlayerController.h"
#include "Player/PNPlayerState.h"

UPNPlayerInputComponent::UPNPlayerInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void UPNPlayerInputComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	const APawn* Owner = GetPawn<APawn>();
	check(Owner);

	const UPNPawnComponent* PawnComponent = Owner->FindComponentByClass<UPNPawnComponent>();
	check(PawnComponent);
	const UPNPawnData* PawnData = PawnComponent->GetPawnData();
	check(PawnData);
	const UPNInputConfig* InputConfig = PawnData->InputConfig;
	check(InputConfig);

	const FPNGameplayTags& GameplayTags = FPNGameplayTags::Get();
	UPNEnhancedInputComponent* PNEnhancedInputComponent = Cast<UPNEnhancedInputComponent>(PlayerInputComponent);
	check(PNEnhancedInputComponent);

	PNEnhancedInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	PNEnhancedInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	PNEnhancedInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_LockOn, ETriggerEvent::Triggered, this, &ThisClass::Input_LockOn);
	PNEnhancedInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_EnableLockOn, ETriggerEvent::Triggered, this, &ThisClass::Input_EnableLockOn);

	PNEnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityPressed, &ThisClass::Input_AbilityReleased);
}

void UPNPlayerInputComponent::EnableInput(bool bIsEnable) const
{
	APawn* Owner = GetPawnChecked<APawn>();
	APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}

	if (bIsEnable)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		Subsystem->AddMappingContext(CameraMappingContext, 0);
	}
	else
	{
		Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
}

void UPNPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Owner = GetPawn<ACharacter>();
	if (Owner == nullptr)
	{
		return;
	}

	EnableInput(true);

	UAbilitySystemComponent* AbilitySystemComponent = Owner->GetPlayerState<APNPlayerState>()->GetAbilitySystemComponent();
	AbilitySystemComponent->AddLooseGameplayTag(FPNGameplayTags::Get().Status_Peace);
	AbilitySystemComponent->AddLooseGameplayTag(FPNGameplayTags::Get().Status_Idle);

	FGameplayTagContainer ActionTagContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(FPNGameplayTags::Get().Action);
	OnActionTagDelegateHandle = AbilitySystemComponent->RegisterGenericGameplayTagEvent().AddUObject(this, &ThisClass::OnUpdateActionTag);

	Owner->OnCharacterMovementUpdated.AddDynamic(this, &ThisClass::OnMovementUpdated);

#ifdef WITH_EDITOR
	// 테스트 용도
	AbilitySystemComponent->AddLooseGameplayTag(FPNGameplayTags::Get().Status_Fight);
#endif
}

void UPNPlayerInputComponent::DestroyComponent(bool bPromoteChildren)
{
	if (APNCharacter* Owner = GetPawn<APNCharacter>())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent())
		{
			AbilitySystemComponent->RegisterGenericGameplayTagEvent().Remove(OnActionTagDelegateHandle);
		}
	}

	Super::DestroyComponent(bPromoteChildren);
}

void UPNPlayerInputComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APNCharacterPlayer* Owner = GetPawnChecked<APNCharacterPlayer>();

	if (UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent())
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Action_Move, 1);
	}

	Owner->MoveByInput(InputActionValue.Get<FVector2D>());

	LastMovementInput = InputActionValue.Get<FVector2D>();
}

void UPNPlayerInputComponent::OnMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	APNCharacter* Owner = GetPawn<APNCharacter>();
	if (Owner == nullptr)
	{
		return;
	}

	if (UPawnMovementComponent* MovementComponent = Owner->GetMovementComponent())
	{
		if (MovementComponent->Velocity.IsNearlyZero())
		{
			if (UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent())
			{
				AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Action_Move, 0);
			}
		}
	}
}

void UPNPlayerInputComponent::Input_Look(const FInputActionValue& InputActionValue)
{
	APawn* Owner = GetPawnChecked<APawn>();
	APNPlayerController* PlayerController = CastChecked<APNPlayerController>(Owner->GetController());
	PlayerController->RotationByInput(InputActionValue.Get<FVector2D>());
}

void UPNPlayerInputComponent::Input_EnableLockOn(const FInputActionValue& InputActionValue)
{
	bIsEnableLockOn = !bIsEnableLockOn;

	if (bIsEnableLockOn == false)
	{
		APawn* Owner = GetPawnChecked<APawn>();
		APNPlayerController* PlayerController = CastChecked<APNPlayerController>(Owner->GetController());
		PlayerController->DisableLockOn();
	}
}

void UPNPlayerInputComponent::Input_LockOn(const FInputActionValue& InputActionValue)
{
	if (bIsEnableLockOn == false)
	{
		return;
	}

	APawn* Owner = GetPawnChecked<APawn>();
	APNPlayerController* PlayerController = CastChecked<APNPlayerController>(Owner->GetController());
	PlayerController->SetLockOnTargetActor();
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

void UPNPlayerInputComponent::OnUpdateActionTag(const FGameplayTag GameplayTag, int32 Count) const
{
	if (GameplayTag.MatchesTag(FPNGameplayTags::Get().Action) == false)
	{
		return;
	}

	APNCharacter* Owner = GetPawn<APNCharacter>();
	if (Owner == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	if (Count > 0)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Status_Idle, 0);
	}
	else if (Owner->IsIdle())
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Status_Idle, 1);
	}
}
