// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNEquipmentComponent.h"

#include "PNStatusActorComponent.h"
#include "DataTable/EquipmentDataTable.h"
#include "DataTable/ItemDataTable.h"
#include "Subsystem/PNGameDataSubsystem.h"

void UPNEquipmentComponent::RequestEquip(const FName ItemKey)
{
	AActor* Owner = GetOwner();
	check(Owner);

	const FItemDataTable* ItemDataTable = UPNGameDataSubsystem::Get()->GetData<FItemDataTable>(ItemKey);
	if (ItemDataTable == nullptr)
	{
		return;
	}

	const FEquipmentDataTable* EquipmentDataTable = UPNGameDataSubsystem::Get()->GetData<FEquipmentDataTable>(ItemDataTable->GetEquipmentKey());
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
