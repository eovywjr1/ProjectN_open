// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "PNAbilitySystemUserInterface.h"
#include "Components/ActorComponent.h"
#include "PNStatusActorComponent.generated.h"

class UPNPawnAttributeSet;

enum class EStatusType : uint8;
enum class EEquipSlotType : uint8;

struct FEquipmentDataTable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNStatusActorComponent : public UActorComponent, public IPNAbilitySystemUserInterface
{
	GENERATED_BODY()

public:
	void ApplyStatusFromEquipment(const FEquipmentDataTable* EquipmentDataTable);
	void UnApplyStatusFromEquipment(const EEquipSlotType EquipSlot);
	
	void RequestHeal(const float HealAmount);
	
	bool IsDead() const;
	
	UFUNCTION(Server, Reliable)
	void ServerRequestAttackDamage(AActor* SourceActor, const AActor* TargetActor);
	
private:
	UPNStatusActorComponent();
	virtual void OnInitializeAbilitySystem(UPNAbilitySystemComponent* AbilitySystemComponent) override final;
	
	FGameplayAttribute GetStatusAttribute(const EStatusType StatusType) const;
	EStatusType GetStatusType(const FGameplayAttribute Attribute) const;
	
	void OnOutOfHp() const;
	void OnDamaged();
	void ApplyAttackDamage(AActor* SourceActor);
	
	void SetPeaceOrFightStatus(const FGameplayTag StatusTag);
	
	void CheckTransitionToPeaceTimerCallback();
	void OnDetected();
	
	void OnActionTagChanged(const FGameplayTag Tag, int32 NewCount);
	
private:
	UPROPERTY()
	TMap<EEquipSlotType, FActiveGameplayEffectHandle> ActiveEquipStatusEffectHandles;
	
	FTimerHandle CheckTransitionToPeaceTimerHandle;
	float NoEnemyDetectTime = 0.0f;
};
