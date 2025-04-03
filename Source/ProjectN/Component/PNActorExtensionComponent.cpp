// Fill out your copyright notice in the Description page of Project Settings.


#include "PNActorExtensionComponent.h"

#include "PNAbilitySystemUserInterface.h"
#include "PNActorGameData.h"
#include "PNCommonModule.h"
#include "AbilitySystem/PNAbilitySet.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "DataTable/MonsterDataTable.h"
#include "Engine/AssetManager.h"
#include "Subsystem/PNGameDataSubsystem.h"

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

	const UAssetManager& AssetManager = UAssetManager::Get();
	

	switch (ActorType)
	{
	case EActorType::Player:
		{
			const FName ActorGameDataFileName = TEXT("PlayerGameData");
			FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(FPrimaryAssetId(FName(TEXT("ActorGameData")), ActorGameDataFileName)));
			if (AssetPtr.IsPending())
			{
				AssetPtr.LoadSynchronous();
			}

			ActorGameData = Cast<UPNActorGameData>(AssetPtr.Get());
			check(ActorGameData);
			
			break;
		}
		
	case EActorType::Monster:
	{
		// Todo. RowName은 임시, 추후 스폰될 때 추가돼야 함
		const FName MonsterDataTableRowName = TEXT("0");
		const FMonsterDataTable* MonsterDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FMonsterDataTable>(TEXT("0"));
		check(MonsterDataTable);
		
		ActorGameData = MonsterDataTable->GetMonsterGameData();
	}
	default:
		{
			break;
		}
	}
	
	if (IsServerActor(GetOwner()) && ActorType < EActorType::Player)
	{
		InitializeAbilitySystem(nullptr, GetOwner());
	}
}
