// Fill out your copyright notice in the Description page of Project Settings.


#include "PNActorExtensionComponent.h"

#include "PNAbilitySystemUserInterface.h"
#include "PNCommonModule.h"
#include "AbilitySystem/PNAbilitySet.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "Actor/PNActorGameData.h"
#include "Actor/PNCharacter.h"

UPNActorExtensionComponent::UPNActorExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;

	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

UPNAbilitySystemComponent* UPNActorExtensionComponent::GetAbilitySystemComponent() const
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = GetOwner()->FindComponentByClass<UPNAbilitySystemComponent>();
	}

	return AbilitySystemComponent;
}

void UPNActorExtensionComponent::InitializeAbilitySystem(UPNAbilitySystemComponent* InAbilitySystemComponent, AActor* InOwnerActor)
{
	check(AbilitySystemComponent == nullptr);

	if (InAbilitySystemComponent == nullptr)
	{
		InAbilitySystemComponent = NewObject<UPNAbilitySystemComponent>(InOwnerActor);
	}

	AActor* Owner = GetOwner();

	AbilitySystemComponent = InAbilitySystemComponent;
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	if (!InAbilitySystemComponent->IsRegistered())
	{
		InAbilitySystemComponent->RegisterComponent();
	}

	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Owner);

	if (Owner->HasAuthority() && ActorGameData)
	{
		for (const UPNAbilitySet* AbilitySet : ActorGameData->AbilitySets)
		{
			if (AbilitySet)
			{
				AbilitySet->GiveAbilityToAbilitySystem(AbilitySystemComponent, this);
			}
		}
	}

	TArray<UActorComponent*> AbilitySystemUserComponents = GetOwner()->GetComponentsByInterface(UPNAbilitySystemUserInterface::StaticClass());
	for (UActorComponent* Component : AbilitySystemUserComponents)
	{
		Cast<IPNAbilitySystemUserInterface>(Component)->OnInitializeAbilitySystem(AbilitySystemComponent);
	}
}

void UPNActorExtensionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// 현재는 캐릭터만 존재하므로 캐릭터에 인터페이스를 추가, Actor가 필요하다면 Actor로 인터페이스를 이동할 계획
	APNCharacter* Owner = Cast<APNCharacter>(GetOwner());
	ActorGameData = Owner->GetActorGameData();

	if (IsServerActor(Owner) && ActorType < EActorType::Player)
	{
		InitializeAbilitySystem(nullptr, Owner);
	}
}
