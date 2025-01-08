// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PNAnimNotify_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PNGameplayTags.h"
#include "PNLogChannels.h"

void UPNAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && EventTag.MatchesTag(FPNGameplayTags::Get().GameplayEvent))
	{
		FGameplayEventData PayloadData;
		PayloadData.EventTag = EventTag;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), PayloadData.EventTag, PayloadData);
	}
}

void UPNAnimNotify_SendGameplayEvent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UPNAnimNotify_SendGameplayEvent, EventTag))
	{
		if (EventTag != FGameplayTag::EmptyTag && EventTag.MatchesTag(FPNGameplayTags::Get().GameplayEvent) == false)
		{
			EventTag = FGameplayTag::EmptyTag;
			UE_LOG(LogPN, Error, TEXT("AnimNotify_SendGameplayEvent에 EventTag에 GameplayEvent로 시작하는 GameplayTag를 입력해야 합니다."));

			Modify();

			FPropertyChangedEvent DummyEvent(PropertyChangedEvent.Property);
			PostEditChangeProperty(DummyEvent);
		}
	}
}
