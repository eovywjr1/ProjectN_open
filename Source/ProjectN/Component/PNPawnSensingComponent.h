// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/PawnSensingComponent.h"
#include "PNPawnSensingComponent.generated.h"

enum class ESenseType : uint8
{
	Default,
	LockOn
};

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNPawnSensingComponent : public UPawnSensingComponent
{
	GENERATED_BODY()
	
public:
	void ConvertSenseType(const ESenseType InDetectType);
	virtual bool CouldSeePawn(const APawn* Other, bool bMaySkipChecks = false) const override final;
	void SetPlayerSensor(const float InCameraFOVAngle);
	
private:
	UPNPawnSensingComponent();
	
	virtual void InitializeComponent() override final;
	
	virtual void UpdateAISensing() override final;
	
	// 청각 감지를 하지 않고, CouldSeePawn에서 콜라이더 탐지 비율을 체크하므로 HasLineOfSightTo가 필요 없기 때문에 SensePawn를 override
	virtual void SensePawn(APawn& Pawn) override final;
	virtual bool ShouldCheckVisibilityOf(APawn* Pawn) const override final;
	
private:
	bool bOnlySenseMonster = false;
};
