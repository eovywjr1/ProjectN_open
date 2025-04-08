// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "GameplayTagContainer.h"
#include "HAL/Platform.h"

class UGameplayTagsManager;

/**
 * FPNGameplayTags
 *
 *	Singleton containing native gameplay tags.
 */
 
struct FPNGameplayTags
{
public:
	FORCEINLINE static const FPNGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);

public:
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Look;
	FGameplayTag InputTag_LockOn;
	FGameplayTag InputTag_NextLockOnTarget;
	FGameplayTag InputTag_ToggleInventory;

	FGameplayTag Status_Dead;
	FGameplayTag Status_Peace;
	FGameplayTag Status_Fight;
	
	FGameplayTag Action;
	FGameplayTag Action_Idle;
	FGameplayTag Action_Jump;
	FGameplayTag Action_Move;
	FGameplayTag Action_Run;
	FGameplayTag Action_Roll;
	FGameplayTag Action_Guard;
	FGameplayTag Action_Attack;
	
	FGameplayTag Ability_Attack;
	
	FGameplayTag GameplayEvent;
	FGameplayTag GameplayEvent_AttackHitCheck;
	FGameplayTag GameplayEvent_EnableComboInput;
	FGameplayTag GameplayEvent_DisableComboInput;
	
	FGameplayTag State_DisableInteraction;
	
private:
	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const FName TagName, const FString TagComment);

private:
	static FPNGameplayTags GameplayTags;
};
