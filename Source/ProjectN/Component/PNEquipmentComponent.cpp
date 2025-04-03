// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNEquipmentComponent.h"

#include "PNSkillComponent.h"
#include "PNStatusActorComponent.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"
#include "DataTable/EquipmentDataTable.h"
#include "DataTable/ItemDataTable.h"
#include "Interface/PNAbilitySystemInterface.h"
#include "Subsystem/PNGameDataSubsystem.h"

void UPNEquipmentComponent::RequestEquip(const FName ItemKey)
{
	if (!CanEquipItem(ItemKey))
	{
		return;
	}

	ServerRequestEquip(ItemKey);
}

void UPNEquipmentComponent::RequestUnEquip(const EEquipSlotType UnEquipSlotType)
{
	UnEquip(UnEquipSlotType);
}

FName UPNEquipmentComponent::GetEquipItemDataTableIndex(const EEquipSlotType EquipSlotType) const
{
	if (!EquipSlots.Contains(EquipSlotType))
	{
		return NAME_None;
	}

	return EquipSlots[EquipSlotType];
}

UClass* UPNEquipmentComponent::GetWeaponAttributeSetClass() const
{
	FName EquipItemDataTableIndex = GetEquipItemDataTableIndex(EEquipSlotType::Weapon);
	check(EquipItemDataTableIndex != NAME_None)

	const FItemDataTable* ItemDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FItemDataTable>(EquipItemDataTableIndex);
	check(ItemDataTable);

	const FEquipmentDataTable* EquipmentDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FEquipmentDataTable>(ItemDataTable->GetEquipmentKey());
	check(EquipmentDataTable)

	UClass* WeaponAttributeSetClass = EquipmentDataTable->GetWeaponAttributeSetClass();
	if (WeaponAttributeSetClass == nullptr)
	{
		return nullptr;
	}
	
	return WeaponAttributeSetClass;
}

UPNEquipmentComponent::UPNEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

	UEnum* EnumPtr = StaticEnum<EEquipSlotType>();
	for (int32 i = 1; i < EnumPtr->NumEnums() - 1; ++i)
	{
		EEquipSlotType EnumValue = static_cast<EEquipSlotType>(i);
		EquipSlots.Add(EnumValue, FName());
	}
}

void UPNEquipmentComponent::Equip(const FName ItemKey)
{
	const FItemDataTable* ItemDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FItemDataTable>(ItemKey);
	const FEquipmentDataTable* EquipmentDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FEquipmentDataTable>(ItemDataTable->GetEquipmentKey());
	const EEquipSlotType EquipSlotType = EquipmentDataTable->GetEquipSlotType();
	
	if (EquipSlots[EquipSlotType].IsNone() == false)
	{
		UnEquip(EquipSlotType);
	}

	EquipSlots[EquipSlotType] = ItemKey;

	AActor* Owner = GetOwner();

	if (IsServerActor(Owner))
	{
		if (UClass* WeaponAttributeSetClass = EquipmentDataTable->GetWeaponAttributeSetClass())
		{
			check(EquipSlotType == EEquipSlotType::Weapon);

			UPNAbilitySystemComponent* AbilitySystemComponent = Cast<IPNAbilitySystemInterface>(Owner)->GetPNAbilitySystemComponent();
			AbilitySystemComponent->InitStats(WeaponAttributeSetClass, nullptr);
		}

		UPNStatusActorComponent* StatusComponent = Owner->FindComponentByClass<UPNStatusActorComponent>();
		StatusComponent->ApplyStatusFromEquipment(EquipmentDataTable);
	}
	
	UPNSkillComponent* SkillComponent = Owner->FindComponentByClass<UPNSkillComponent>();
	SkillComponent->OnEquipWeapon();
}

void UPNEquipmentComponent::ServerRequestEquip_Implementation(const FName ItemKey)
{
	if (!CanEquipItem(ItemKey))
	{
		return;
	}
	
	Equip(ItemKey);
	ClientNotifyEquip(ItemKey);
}

void UPNEquipmentComponent::OnInitializeAbilitySystem(UPNAbilitySystemComponent* AbilitySystemComponent)
{
	// Todo. 추후 무기 장착/획득할 때 넣어야 함
	AActor* Owner = GetOwner();
	if (IsClientActor(Owner))
	{
		RequestEquip(TEXT("0"));
	}
}

bool UPNEquipmentComponent::CanEquipItem(const FName ItemKey) const
{
	const FItemDataTable* ItemDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FItemDataTable>(ItemKey);
	if (ItemDataTable == nullptr)
	{
		return false;
	}

	const FEquipmentDataTable* EquipmentDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FEquipmentDataTable>(ItemDataTable->GetEquipmentKey());
	if (EquipmentDataTable == nullptr)
	{
		return false;
	}
	
	return true;
}

void UPNEquipmentComponent::ClientNotifyEquip_Implementation(const FName ItemKey)
{
	Equip(ItemKey);
}

void UPNEquipmentComponent::UnEquip(EEquipSlotType UnEquipSlotType)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (UnEquipSlotType == EEquipSlotType::Invalid || EquipSlots[UnEquipSlotType].IsNone())
	{
		return;
	}

	UPNStatusActorComponent* StatusComponent = Owner->FindComponentByClass<UPNStatusActorComponent>();
	StatusComponent->UnApplyStatusFromEquipment(UnEquipSlotType);

	// 추후 장비 해제할 때 인벤토리 관련 확정나면 구현 필요

	EquipSlots[UnEquipSlotType] = FName();
}
