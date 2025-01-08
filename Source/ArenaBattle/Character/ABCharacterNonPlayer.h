// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/ABCharacterAIInterface.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(Config=ArenaBattle)
class ARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase, public IABCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterNonPlayer();

private:
	virtual void PostInitializeComponents() override final;
	
	virtual void SetDead() override final;
	void NPCMeshLoadCompleted() const;
	
	virtual float GetAIPatrolRadius() override final;
	virtual float GetAIDetectRange() override final;
	virtual float GetAIAttackRange() override final;
	virtual float GetAIAITurnSpeed() override final;
	
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override final;
	virtual void AttackByAI() override final;
	
	virtual void NotifyComboActionEnd() override final;
	
private:
	UPROPERTY(Config)
	TArray<FSoftObjectPath> NPCMeshes;
	
	TSharedPtr<FStreamableHandle> NPCMeshHandle;
	
	FAICharacterAttackFinished OnAttackFinished;
};
