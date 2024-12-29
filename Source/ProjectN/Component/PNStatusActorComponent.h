// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ActiveGameplayEffectHandle.h"
#include "Components/ActorComponent.h"
#include "PNStatusActorComponent.generated.h"

class UPNPawnAttributeSet;

enum class EStatusType : uint8;
enum class EEquipSlotType : uint8;

struct FEquipmentDataTable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNStatusActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void ApplyStatusFromEquipment(const FEquipmentDataTable* EquipmentDataTable);
	void UnApplyStatusFromEquipment(const EEquipSlotType EquipSlot);
	
	void OnPawnAttributeSetChanged(FGameplayAttribute Attribute);
	
	void RequestHeal(const float HealAmount);

private:
	virtual void BeginPlay() override final;
	
	FGameplayAttribute GetStatusAttribute(const EStatusType StatusType) const;
	EStatusType GetStatusType(const FGameplayAttribute Attribute) const;
	
	void OnOutOfHp();
	
private:
	UPROPERTY()
	TMap<EEquipSlotType, FActiveGameplayEffectHandle> ActiveEquipStatusEffectHandles;
};
