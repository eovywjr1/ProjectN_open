// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PNAnimNotify_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UPNAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual FString GetNotifyName_Implementation() const override final { return TEXT("AnimNotify_SendGameplayEvent"); }
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override final;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override final;
#endif
	
private:
	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;
};
