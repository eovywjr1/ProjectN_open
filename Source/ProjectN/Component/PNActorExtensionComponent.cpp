// Fill out your copyright notice in the Description page of Project Settings.


#include "PNActorExtensionComponent.h"

#include "PNActorGameData.h"
#include "AbilitySystem/PNAbilitySet.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "Interface/PNAbilitySystemInterface.h"

UPNActorExtensionComponent::UPNActorExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;

	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UPNActorExtensionComponent::InitializeAbilitySystem(UPNAbilitySystemComponent* InAbilitySystemComponent, AActor* InOwnerActor)
{
	if (AbilitySystemComponent)
	{
		return;
	}

	if (ActorType < EActorType::Player)
	{
		InAbilitySystemComponent = NewObject<UPNAbilitySystemComponent>(InOwnerActor);
	}

	if (!InAbilitySystemComponent->IsRegistered())
	{
		InAbilitySystemComponent->RegisterComponent();
	}

	AActor* Owner = GetOwner();

	AbilitySystemComponent = InAbilitySystemComponent;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Owner);
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

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

	IPNAbilitySystemInterface* AbilitySystemInterface = GetOwner<IPNAbilitySystemInterface>();
	check(AbilitySystemInterface);
	AbilitySystemInterface->OnInitializeAbilitySystemDelegate.Broadcast();
}

void UPNActorExtensionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	const UAssetManager& AssetManager = UAssetManager::Get();
	FName ActorGameDataFileName;

	switch (ActorType)
	{
	case EActorType::Player:
		{
			ActorGameDataFileName = TEXT("PlayerGameData");
			break;
		}
	default:
		{
			break;
		}
	}
	
	if(!ActorGameDataFileName.IsNone())
	{
		FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(FPrimaryAssetId(FName(TEXT("ActorGameData")), ActorGameDataFileName)));
		if (AssetPtr.IsPending())
		{
			AssetPtr.LoadSynchronous();
		}

		ActorGameData = Cast<UPNActorGameData>(AssetPtr.Get());
		check(ActorGameData);
	}
}

void UPNActorExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (ActorType < EActorType::Player)
	{
		InitializeAbilitySystem(nullptr, GetOwner());
	}
}
