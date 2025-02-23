// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PNGameplayAbility_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PNGameplayTags.h"
#include "PNLogChannels.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"
#include "AbilitySystem/TargetActor/PNTargetActor_HitCheckActor.h"
#include "AbilitySystem/Task/PNAbilityTask_TraceToPawn.h"
#include "Component/PNSkillComponent.h"
#include "Component/PNStatusActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPNGameplayAbility_Attack::UPNGameplayAbility_Attack()
	: BaseAttackAbilityTag(FGameplayTag()),
	  ChargeAttackAbilityTag(FGameplayTag())
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;

	ActivationBlockedTags.AddTag(FPNGameplayTags::Get().Ability_Attack);
}

bool UPNGameplayAbility_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (!ActorInfo->AvatarActor->IsA(ACharacter::StaticClass()))
	{
		return false;
	}

	return true;
}

void UPNGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FPNGameplayTags::Get().GameplayEvent_AttackHitCheck);
	WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEvent);
	WaitEventTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEnableComboInputEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FPNGameplayTags::Get().GameplayEvent_EnableComboInput);
	WaitEnableComboInputEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEvent);
	WaitEnableComboInputEventTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitDisableComboInputEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FPNGameplayTags::Get().GameplayEvent_DisableComboInput);
	WaitDisableComboInputEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEvent);
	WaitDisableComboInputEventTask->ReadyForActivation();

	if (bEnableCharge)
	{
		UAbilityTask_WaitDelay* WaitEnableChargeDelayEventTask = UAbilityTask_WaitDelay::WaitDelay(this, ChargeRequirementTime);
		WaitEnableChargeDelayEventTask->OnFinish.AddDynamic(this, &ThisClass::OnTransitionChargeTimerCallback);
		WaitEnableChargeDelayEventTask->ReadyForActivation();
	}

	if (AttackInvokeInputTimingType == EAttackInvokeInputTimingType::InputPressed)
	{
		ExecuteAttack();
	}
}

void UPNGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	bChargeAttack = false;
	AttackData = nullptr;

	Cast<ACharacter>(ActorInfo->AvatarActor.Get())->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FPNGameplayTags::Get().Action_Attack, 1);
	EnableExecuteAttack();

	if (!bWasCancelled)
	{
		if (UPNSkillComponent* SkillComponent = GetAvatarActorFromActorInfo()->FindComponentByClass<UPNSkillComponent>())
		{
			SkillComponent->ClearCombo();
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPNGameplayAbility_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (AttackInvokeInputTimingType == EAttackInvokeInputTimingType::InputPressed)
	{
		ExecuteAttack();
	}
}

void UPNGameplayAbility_Attack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (AttackInvokeInputTimingType == EAttackInvokeInputTimingType::InputReleased)
	{
		ExecuteAttack();
	}
}

void UPNGameplayAbility_Attack::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UPNGameplayAbility_Attack::OnInterruptedCallback()
{
	if (GetAvatarActorFromActorInfo()->FindComponentByClass<UPNSkillComponent>()->IsCurrentCombo(AttackData->AttackTag))
	{
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UPNGameplayAbility_Attack::OnTransitionChargeTimerCallback()
{
	bChargeAttack = true;
}

void UPNGameplayAbility_Attack::ExecuteAttack()
{
	if (!IsEnableExecuteAttack())
	{
		return;
	}

	// Charge가 불가능할 경우 기본 공격이 수행
	FGameplayTag AttackTag = BaseAttackAbilityTag;
	if (IsEnableChargeAttack())
	{
		AttackTag = ChargeAttackAbilityTag;
	}

	if (UPNSkillComponent* SkillComponent = GetAvatarActorFromActorInfo()->FindComponentByClass<UPNSkillComponent>())
	{
		AttackData = SkillComponent->ExecuteNextCombo(AttackTag);
		if (AttackData == nullptr)
		{
			return;
		}
	}

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	AbilitySystemComponent->SetLooseGameplayTagCount(AttackTag, 1);

	AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Action_Attack, 1);
	Cast<ACharacter>(GetAvatarActorFromActorInfo())->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ExecuteAttack"), AttackData->AttackActionMontage, 1.0f, AttackData->AttackActionMontageSectionName);
	PlayAttackTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void UPNGameplayAbility_Attack::OnGameplayEvent(FGameplayEventData Payload)
{
	if (Payload.EventTag == FPNGameplayTags::Get().GameplayEvent_AttackHitCheck)
	{
		AttackHitCheck();
	}
	else if (Payload.EventTag == FPNGameplayTags::Get().GameplayEvent_EnableComboInput)
	{
		EnableExecuteAttack();
	}
	else if (Payload.EventTag == FPNGameplayTags::Get().GameplayEvent_DisableComboInput)
	{
		DisableExecuteAttack();
	}
}

void UPNGameplayAbility_Attack::AttackHitCheck()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Checked();
	FGameplayTagContainer TagContainer;
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);

	FGameplayTag AttackTag;
	for (const FGameplayTag& GameplayTag : TagContainer)
	{
		if (GameplayTag.MatchesTag(FPNGameplayTags::Get().Ability_Attack))
		{
			AttackTag = GameplayTag;
			break;
		}
	}

	const UPNWeaponAttributeSet* WeaponAttributeSet = GetAbilitySystemComponentFromActorInfo()->GetSet<UPNWeaponAttributeSet>();
	check(WeaponAttributeSet);
	FHitBoxData AttackHitBoxData;
	if (!WeaponAttributeSet->GetAttackHitBoxData(AttackTag, AttackHitBoxData))
	{
		UE_LOG(LogPN, Warning, TEXT("무기 AttributeSet에 %s 태그의 공격 어빌리티의 판정 범위 데이터가 없습니다."), *AttackTag.ToString());
	}

	if (TargetActorHitCheckClass)
	{
		UPNAbilityTask_TraceToPawn* AttackTraceTask = UPNAbilityTask_TraceToPawn::CreateTask(this, TargetActorHitCheckClass, AttackHitBoxData);
		AttackTraceTask->OnComplete.AddUObject(this, &ThisClass::OnAttackHitTraceResultCallback);
		AttackTraceTask->ReadyForActivation();
	}
}

void UPNGameplayAbility_Attack::OnAttackHitTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	APawn* AvatarActor = Cast<APawn>(GetAvatarActorFromActorInfo());
	UPNStatusActorComponent* AvatarStatusActorComponent = AvatarActor->FindComponentByClass<UPNStatusActorComponent>();
	TArray<AActor*> TargetActors = UAbilitySystemBlueprintLibrary::GetAllActorsFromTargetData(TargetDataHandle);
	for (AActor* TargetActor : TargetActors)
	{
		APawn* TargetActorCast = Cast<APawn>(TargetActor);
		if (TargetActorCast == nullptr)
		{
			continue;
		}

		check(AvatarActor->IsPlayerControlled() || TargetActorCast->IsPlayerControlled());

		if (AvatarActor->IsPlayerControlled())
		{
			AvatarStatusActorComponent->ServerRequestAttackDamage(AvatarActor, TargetActorCast);
		}
		else if (TargetActorCast->IsPlayerControlled())
		{
			UPNStatusActorComponent* TargetStatusActorComponent = TargetActorCast->FindComponentByClass<UPNStatusActorComponent>();
			TargetStatusActorComponent->ServerRequestAttackDamage(AvatarActor, TargetActorCast);
		}
	}

	const bool bHit = !TargetActors.IsEmpty();
	UPNSkillComponent* AvatarSkillActorComponent = AvatarActor->FindComponentByClass<UPNSkillComponent>();
	AvatarSkillActorComponent->ServerPostSkillProcess(bHit);
}

bool UPNGameplayAbility_Attack::IsEnableExecuteAttack() const
{
	return GetAbilitySystemComponentFromActorInfo()->GetTagCount(FPNGameplayTags::Get().Ability_Attack) <= 0;
}

void UPNGameplayAbility_Attack::EnableExecuteAttack() const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Ability_Attack, 0);

	if (AttackData)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(AttackData->AttackTag, 0);
	}
}

void UPNGameplayAbility_Attack::DisableExecuteAttack() const
{
	GetAbilitySystemComponentFromActorInfo()->SetLooseGameplayTagCount(FPNGameplayTags::Get().Ability_Attack, 1);
}

bool UPNGameplayAbility_Attack::IsEnableChargeAttack() const
{
	if (!bChargeAttack)
	{
		return false;
	}

	const bool bEnableCombo = GetAvatarActorFromActorInfo()->FindComponentByClass<UPNSkillComponent>()->IsEnableNextCombo(ChargeAttackAbilityTag);
	if (!bEnableCombo)
	{
		return false;
	}

	return true;
}
