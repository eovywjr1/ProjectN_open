// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "PNDetectComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDetectedDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTN_API UPNDetectComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	void SetTargetNextPriorityEnemy();

	FORCEINLINE APawn* GetTargetedEnemy() const { return TargetedEnemy; }
	
	bool CanDetectEnemy(const APawn* Enemy) const;

private:
	UPNDetectComponent(const FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override final;
	virtual void BeginPlay() override final;
	
	void SetTargetEnemy(APawn* InDetectedEnemy);
	
	void DetectInteractableActor() const;
	
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
	
	void UpdateDetectedPawns();
	
public:
	FOnDetectedDelegate OnDetectedDelegate;
	
private:
	// 탐지된 적을 추가 횟수가 다음 우선순위 타겟을 정하기 위한 정렬 횟수보다 많다고 판단하여 TSet으로 결정
	TSet<APawn*> DetectedEnemies;
	APawn* TargetedEnemy = nullptr;
};
