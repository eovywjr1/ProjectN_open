// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "PNGameplayTags.h"
#include "Interface/PNAbilitySystemInterface.h"

bool UPNCharacterMovementComponent::IsIdle() const
{
	if (const bool bIsMoving = Velocity.IsNearlyZero() == false || IsFalling())
	{
		return false;
	}

	return true;
}

void UPNCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	if (IPNAbilitySystemInterface* AbilitySystemInterface = GetOwner<IPNAbilitySystemInterface>())
	{
		AbilitySystemInterface->OnInitializeAbilitySystemDelegate.AddUObject(this, &ThisClass::OnInitializeAbilitySystem);
	}
}

void UPNCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	if (IPNAbilitySystemInterface* OwnerAbilitySystemInterface = Cast<IPNAbilitySystemInterface>(GetOwner()))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = OwnerAbilitySystemInterface->GetAbilitySystemComponent())
		{
			if (IsIdle())
			{
				AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Action_Move, 0);
			}
			else
			{
				AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Action_Move, 1);
			}
		}
	}
}

void UPNCharacterMovementComponent::OnUpdateTag(const FGameplayTag GameplayTag, int32 Count) const
{
	if (GameplayTag.MatchesTag(FPNGameplayTags::Get().Action) && GameplayTag.MatchesTagExact(FPNGameplayTags::Get().Action) == false && GameplayTag.MatchesTagExact(FPNGameplayTags::Get().Action_Idle) == false)
	{
		IPNAbilitySystemInterface* OwnerAbilitySystemInterface = Cast<IPNAbilitySystemInterface>(GetOwner());
		check(OwnerAbilitySystemInterface);

		UAbilitySystemComponent* AbilitySystemComponent = OwnerAbilitySystemInterface->GetAbilitySystemComponent();
		check(AbilitySystemComponent);

		if (Count > 0)
		{
			AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Action_Idle, 0);
		}
		else if (IsIdle())
		{
			AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Action_Idle, 1);
		}
	}
}

void UPNCharacterMovementComponent::OnInitializeAbilitySystem()
{
	IPNAbilitySystemInterface* OwnerAbilitySystemInterface = Cast<IPNAbilitySystemInterface>(GetOwner());
	UAbilitySystemComponent* AbilitySystemComponent = OwnerAbilitySystemInterface->GetAbilitySystemComponent();

	AbilitySystemComponent->AddLooseGameplayTag(FPNGameplayTags::Get().Action_Idle);
	OnActionTagDelegateHandle = AbilitySystemComponent->RegisterGenericGameplayTagEvent().AddUObject(this, &ThisClass::OnUpdateTag);
}
