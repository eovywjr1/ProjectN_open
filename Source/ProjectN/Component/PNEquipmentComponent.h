// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PNEquipmentComponent.generated.h"

enum class EEquipSlotType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	void RequestEquip(const FName ItemKey);
	void RequestUnEquip(const EEquipSlotType UnEquipSlotType);

private:
	UPNEquipmentComponent();
	virtual void InitializeComponent() override final;
	
	void OnInitializeAbilitySystem();
	
	void UnEquip(EEquipSlotType UnEquipSlotType);
	
private:
	TMap<EEquipSlotType, FName> EquipSlots;
};
