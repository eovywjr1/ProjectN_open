// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PNAnimNotify_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual FString GetNotifyName_Implementation() const override final;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override final;
};
