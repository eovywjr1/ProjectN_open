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
	AActor* Owner = GetOwner();
	check(Owner);

	const FItemDataTable* ItemDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FItemDataTable>(ItemKey);
	if (ItemDataTable == nullptr)
	{
		return;
	}

	const FEquipmentDataTable* EquipmentDataTable = UPNGameDataSubsystem::Get(GetWorld())->GetData<FEquipmentDataTable>(ItemDataTable->GetEquipmentKey());
	if (EquipmentDataTable == nullptr)
	{
		return;
	}

	const EEquipSlotType EquipSlotType = EquipmentDataTable->GetEquipSlotType();
	if (EquipSlots[EquipSlotType].IsNone() == false)
	{
		UnEquip(EquipSlotType);
	}

	EquipSlots[EquipSlotType] = ItemKey;

	if (UPNAttributeSet* EquipAttributeSet = EquipmentDataTable->GetWeaponAttributeSet())
	{
		check(EquipSlotType == EEquipSlotType::Weapon);

		GetOwner<IPNAbilitySystemInterface>()->GetAbilitySystemComponent()->AddSpawnedAttribute(EquipAttributeSet);
		GetOwner()->FindComponentByClass<UPNSkillComponent>()->OnEquipWeapon();
	}

	UPNStatusActorComponent* StatusComponent = Owner->FindComponentByClass<UPNStatusActorComponent>();
	StatusComponent->ApplyStatusFromEquipment(EquipmentDataTable);

	// 추후 장비 장착할 때 인벤토리 관련 확정나면 구현 필요
}

void UPNEquipmentComponent::RequestUnEquip(const EEquipSlotType UnEquipSlotType)
{
	UnEquip(UnEquipSlotType);
}

UPNEquipmentComponent::UPNEquipmentComponent()
{
	UEnum* EnumPtr = StaticEnum<EEquipSlotType>();
	for (int32 i = 1; i < EnumPtr->NumEnums() - 1; ++i)
	{
		EEquipSlotType EnumValue = static_cast<EEquipSlotType>(i);
		EquipSlots.Add(EnumValue, FName());
	}
}

void UPNEquipmentComponent::OnInitializeAbilitySystem(UPNAbilitySystemComponent* AbilitySystemComponent)
{
	// Todo. 추후 무기 장착/획득할 때 넣어야 함
	if (TSubclassOf<UPNWeaponAttributeSet> WeaponAttributeSetClass = LoadClass<UPNWeaponAttributeSet>(this, TEXT("/Script/Engine.Blueprint'/Game/ProjectN/Weapon/BP_BasicWeaponAttributeSet.BP_BasicWeaponAttributeSet'")))
	{
		if (UPNWeaponAttributeSet* WeaponAttributeSet = NewObject<UPNWeaponAttributeSet>(this, WeaponAttributeSetClass))
		{
			AbilitySystemComponent->AddSpawnedAttribute(WeaponAttributeSet);
			GetOwner()->FindComponentByClass<UPNSkillComponent>()->OnEquipWeapon();
		}
	}
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
