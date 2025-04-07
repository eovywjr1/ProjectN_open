// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PNInventoryComponent.generated.h"

USTRUCT()
struct FPNInventorySlot
{
	GENERATED_BODY()

public:
	FPNInventorySlot();
	FPNInventorySlot(const FName InItemKey, uint8 InStackCount);

	FORCEINLINE const FName& GetItemKey() const { return ItemKey; }
	FORCEINLINE uint8 GetStackCount() const { return StackCount; }
	FORCEINLINE void AddStackCount(const uint8 InStackCount) { StackCount += InStackCount; }
	FORCEINLINE void SubtractionStackCount(const uint8 InStackCount) { StackCount = FMath::Clamp(StackCount - InStackCount, 0, StackCount); }
	
	FORCEINLINE bool operator==(const FPNInventorySlot& Other) const { return ItemKey == Other.ItemKey; }
	
private:
	UPROPERTY()
	FName ItemKey;

	UPROPERTY()
	uint8 StackCount = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	void RequestAddItem(const FName ItemKey, const uint8 Count);
	void RequestRemoveItem(const FName ItemKey, const uint8 Count);
	void RequestUseItem(const FName ItemKey, const uint8 Count);

private:
	UPNInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override final;

	UFUNCTION(Server, Reliable)
	void ServerAddItem(const FName ItemKey, const uint8 Count);
	
	uint8 RemoveItem(FPNInventorySlot* ItemSlot, const uint8 Count);

	UFUNCTION()
	void OnRep_Slots();

	bool IsValidItem(const FName ItemKey) const;
	bool IsEnableAddItem(const FName ItemKey, const uint8 Count) const;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_Slots)
	TArray<FPNInventorySlot> Slots;
};