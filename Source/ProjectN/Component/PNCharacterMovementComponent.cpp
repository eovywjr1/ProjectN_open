// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "PNGameplayTags.h"

bool UPNCharacterMovementComponent::IsIdle() const
{
	if (const bool bIsMoving = Velocity.IsNearlyZero() == false || IsFalling())
	{
		return false;
	}

	return true;
}

void UPNCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IAbilitySystemInterface* OwnerAbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		UAbilitySystemComponent* AbilitySystemComponent = OwnerAbilitySystemInterface->GetAbilitySystemComponent();
		check(AbilitySystemComponent);

		AbilitySystemComponent->AddLooseGameplayTag(FPNGameplayTags::Get().Action_Idle);
		OnActionTagDelegateHandle = AbilitySystemComponent->RegisterGenericGameplayTagEvent().AddUObject(this, &ThisClass::OnUpdateTag);
	}
}

void UPNCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	if (IAbilitySystemInterface* OwnerAbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		UAbilitySystemComponent* AbilitySystemComponent = OwnerAbilitySystemInterface->GetAbilitySystemComponent();
		check(AbilitySystemComponent);

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

void UPNCharacterMovementComponent::OnUpdateTag(const FGameplayTag GameplayTag, int32 Count) const
{
	if (GameplayTag.MatchesTag(FPNGameplayTags::Get().Action) && GameplayTag.MatchesTagExact(FPNGameplayTags::Get().Action) == false && GameplayTag.MatchesTagExact(FPNGameplayTags::Get().Action_Idle) == false)
	{
		IAbilitySystemInterface* OwnerAbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner());
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
