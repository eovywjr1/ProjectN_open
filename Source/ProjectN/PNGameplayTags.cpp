// Copyright Epic Games, Inc. All Rights Reserved.

#include "PNGameplayTags.h"

#include "GameplayTagsManager.h"
#include "PNLogChannels.h"

FPNGameplayTags FPNGameplayTags::GameplayTags;

void FPNGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

	// Notify manager that we are done adding native tags.
	Manager.DoneAddingNativeTags();
}

void FPNGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_Move, TEXT("InputTag.Move"), TEXT("Move input."));
	AddTag(InputTag_Jump, TEXT("InputTag.Jump"), TEXT("Jump input."));
	AddTag(InputTag_Look, TEXT("InputTag.Look"), TEXT("Look input."));
	AddTag(InputTag_LockOn, TEXT("InputTag.LockOn"), TEXT("LockOn input."));
	AddTag(InputTag_EnableLockOn, TEXT("InputTag.EnableLockOn"), TEXT("EnableLockOn input."));

	AddTag(Status_Idle, TEXT("Status.Idle"), TEXT("Status Idle"));
	AddTag(Status_Dead, TEXT("Status.Dead"), TEXT("Status Dead"));
	AddTag(Status_Peace, TEXT("Status.Peace"), TEXT("Status Peace"));
	AddTag(Status_Fight, TEXT("Status.Fight"), TEXT("Status Fight"));
	
	AddTag(Action, TEXT("Action"), TEXT("Action"));
	AddTag(Action_Jump, TEXT("Action.Jump"), TEXT("Action Jump"));
	AddTag(Action_Move, TEXT("Action.Move"), TEXT("Action Move"));
	AddTag(Action_Run, TEXT("Action.Run"), TEXT("Action Run"));
	AddTag(Action_Roll, TEXT("Action.Roll"), TEXT("Action Roll"));
	AddTag(Action_Guard, TEXT("Action.Guard"), TEXT("Action Guard"));
	AddTag(Action_Attack, TEXT("Action.Attack"), TEXT("Action Attack"));
	
	AddTag(GameplayEvent_AttackHitCheck, TEXT("GameplayEvent.AttackHitCheck"), TEXT("GameplayEvent AttackHitCheck"));
}

void FPNGameplayTags::AddTag(FGameplayTag& OutTag, const FName TagName, const FString TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(TagName, FString(TEXT("(Native) ")) + TagComment);
}

FGameplayTag FPNGameplayTags::FindTagByString(FString TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	check(Tag.IsValid() || bMatchPartialString);

	if (Tag.IsValid() == false && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
}
