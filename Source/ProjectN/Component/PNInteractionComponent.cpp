// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNInteractionComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "PNGameplayTags.h"
#include "DataTable/InteractionDataTable.h"
#include "Subsystem/PNGameDataSubsystem.h"

bool UPNInteractionComponent::CanInteraction() const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwner<IAbilitySystemInterface>()->GetAbilitySystemComponent();
	if (AbilitySystemComponent->GetGameplayTagCount(FPNGameplayTags::Get().State_DisableInteraction) > 0)
	{
		return false;
	}

	return true;
}

FName UPNInteractionComponent::GetInteractionKey()
{
	if (!CanInteraction())
	{
		return NAME_None;
	}

	// Todo. 현재 테스트 용도, 인터렉션 데이터테이블 키를 NPC/Gimmick 데이터테이블 등에서 가져와야 함
	FName InteractionDataTableKey = FName(TEXT("Test"));

	return InteractionDataTableKey;
}

void UPNInteractionComponent::RequestInteraction(const FObjectKey InteractionTargetActorKey, const FName InteractionKey)
{
	AActor* InteractionTargetActor = Cast<AActor>(InteractionTargetActorKey.ResolveObjectPtr());
	if (!IsValid(InteractionTargetActor))
	{
		return;
	}

	ServerProcessInteraction(InteractionTargetActor, InteractionKey);
}

void UPNInteractionComponent::TryActivateInteractionAbility(const FGameplayTagContainer& InteractionTag) const
{
	GetOwner<IAbilitySystemInterface>()->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(InteractionTag, true);
}

UPNInteractionComponent::UPNInteractionComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPNInteractionComponent::ServerProcessInteraction_Implementation(AActor* InteractionTargetActor, const FName InteractionKey)
{
	const FInteractionDataTable* InteractionDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FInteractionDataTable>(InteractionKey);
	if (InteractionDataTable == nullptr)
	{
		return;
	}

	if (!CanInteraction())
	{
		return;
	}

	const FGameplayTagContainer InteractionTag(InteractionDataTable->GetInteractionAbilityTag());
	InteractionTargetActor->FindComponentByClass<UPNInteractionComponent>()->TryActivateInteractionAbility(InteractionTag);
}
