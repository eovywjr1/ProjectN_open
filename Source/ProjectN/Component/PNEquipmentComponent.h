// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNAbilitySystemUserInterface.h"
#include "Components/PawnComponent.h"
#include "PNEquipmentComponent.generated.h"

enum class EEquipSlotType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNEquipmentComponent : public UPawnComponent, public IPNAbilitySystemUserInterface
{
	GENERATED_BODY()
	
public:
	void RequestEquip(const FName ItemKey);
	void RequestUnEquip(const EEquipSlotType UnEquipSlotType);
	
	FName GetEquipItemDataTableIndex(const EEquipSlotType EquipSlotType) const;

private:
	UPNEquipmentComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnInitializeAbilitySystem(UPNAbilitySystemComponent* AbilitySystemComponent) override final;
	
	bool CanEquipItem(const FName ItemKey) const; 
	void Equip(const FName ItemKey);
	
	UFUNCTION(Server, Reliable)
	void ServerRequestEquip(const FName ItemKey);
	
	UFUNCTION(Client, Reliable)
	void ClientNotifyEquip(const FName ItemKey);
	
	void UnEquip(EEquipSlotType UnEquipSlotType);
	
private:
	TMap<EEquipSlotType, FName> EquipSlots;
};
