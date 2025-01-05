// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PNHUD.generated.h"

enum class EStatusType : uint8;

UENUM()
enum class EUIControllerID : uint8
{
	HUD
};

USTRUCT()
struct FUIControllerData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ControllerClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> Controller;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 Layer = 0;
};

// 이벤트를 발생시키는 행동을 기준으로 델리게이트 이름 작성
DECLARE_MULTICAST_DELEGATE_TwoParams(GameEvent_OnStatusChangedDelegate, FObjectKey, EStatusType);
DECLARE_MULTICAST_DELEGATE_OneParam(GameEvent_OnInitStatusDelegate, FObjectKey);

DECLARE_MULTICAST_DELEGATE_OneParam(GameEvent_OnSetLockOnTargetDelegate, FObjectKey);
DECLARE_MULTICAST_DELEGATE(GameEvent_OnDeactivatedLockOnDelegate);

/**
 * 
 */
UCLASS()
class PROJECTN_API APNHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	GameEvent_OnStatusChangedDelegate OnStatusChangedDelegate;
	GameEvent_OnInitStatusDelegate OnInitStatusDelegate;
	
	GameEvent_OnSetLockOnTargetDelegate OnSetLockOnTargetDelegate;
	GameEvent_OnDeactivatedLockOnDelegate OnDeactivatedLockOnDelegate;
	
private:
	virtual void BeginPlay() override final;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<EUIControllerID, FUIControllerData> Controllers;
};
