// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PNAnimNotify_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PNGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"

FString UPNAnimNotify_AttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("AttackHitCheck");
}

void UPNAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			FGameplayEventData PayloadData;
			PayloadData.EventTag = FPNGameplayTags::Get().GameplayEvent_AttackHitCheck;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, PayloadData.EventTag, PayloadData);
		}
	}
}
