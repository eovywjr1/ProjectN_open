// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PNDetectComponent.generated.h"

enum class EDetectType : uint8
{
	Default,
	LockOn
};

DECLARE_MULTICAST_DELEGATE(FOnDetectedDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNDetectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void DetectNextPriorityEnemy();

	FORCEINLINE const AActor* GetDetectedEnemy() const { return DetectedEnemy; }
	void SetDetectTypeAndUpdateDetect(const EDetectType InDetectType);

private:
	UPNDetectComponent();
	virtual void BeginPlay() override final;
	
	void UpdateDetectedEnemy();
	void DetectEnemy(TArray<const AActor*>& InSortedDetectedEnemies) const;
	bool IsDetectableEnemy(const AActor* Enemy) const;
	void SetDetectedEnemy(const AActor* InDetectedEnemy);
	
public:
	FOnDetectedDelegate OnDetectedDelegate;
	
private:
	TObjectPtr<const AActor> DetectedEnemy = nullptr;
	FTimerHandle UpdateDetectEnemyTimerHandle;
	float CheckDetectEnemyDistance;
};
