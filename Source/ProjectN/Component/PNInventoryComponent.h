// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PNInventoryComponent.generated.h"

struct FPNInventorySlot
{
public:
	FPNInventorySlot(const FName InItemKey, uint8 InStackCount);

	FORCEINLINE FName GetItemKey() const { return ItemKey; }
	FORCEINLINE uint8 GetStackCount() const { return StackCount; }
	FORCEINLINE void AddStackCount(const uint8 InStackCount) { StackCount += InStackCount; }
	FORCEINLINE void SubtractionStackCount(const uint8 InStackCount) { StackCount = FMath::Clamp(StackCount - InStackCount, 0, StackCount); }
	
	FORCEINLINE bool operator==(const FPNInventorySlot& Other) const { return ItemKey == Other.ItemKey; }
	
private:
	FName ItemKey;
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
	
	uint8 RemoveItem(FPNInventorySlot* ItemSlot, const uint8 Count);
	
private:
	// Todo. 현재는 인벤토리 슬롯이 적을 것 같아 TArray만 사용하지만 추후 TMap을 같이 사용할 수도 있음
	TArray<FPNInventorySlot> Inventory;
};
