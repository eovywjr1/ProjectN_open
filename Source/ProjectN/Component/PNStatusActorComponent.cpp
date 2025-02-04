// Fill out your copyright notice in the Description page of Project Settings.


#include "PNStatusActorComponent.h"

#include "PNDetectComponent.h"
#include "PNGameplayTags.h"
#include "AbilitySystem/PNAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PNPlayerAttributeSet.h"
#include "Actor/PNCharacter.h"
#include "DataTable/EquipmentDataTable.h"
#include "DataTable/StatusDataTable.h"
#include "Subsystem/PNGameDataSubsystem.h"
#include "UI/PNHUD.h"

constexpr float NoDetectionEnemyToPeaceTime = 5.0f;

void UPNStatusActorComponent::ApplyStatusFromEquipment(const FEquipmentDataTable* EquipmentDataTable)
{
	APNCharacter* Owner = GetOwner<APNCharacter>();
	check(Owner);

	UPNAbilitySystemComponent* AbilitySystemComponent = Cast<UPNAbilitySystemComponent>(Owner->GetAbilitySystemComponent());
	check(AbilitySystemComponent);

	if (EquipmentDataTable == nullptr)
	{
		return;
	}

	const UPNGameDataSubsystem* DataSubsystem = UPNGameDataSubsystem::Get(GetWorld());
	UGameplayEffect* EquipmentStatusEffect = NewObject<UGameplayEffect>(this, FName(TEXT("EquipmentStatusEffect")));
	EquipmentStatusEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;

	for (const FName StatusKey : EquipmentDataTable->GetStatusKeys())
	{
		if (StatusKey.IsNone())
		{
			continue;
		}

		const FStatusDataTable* StatusDataTable = DataSubsystem->GetData<FStatusDataTable>(StatusKey);
		if (StatusDataTable == nullptr)
		{
			continue;
		}

		const FGameplayAttribute StatusAttribute = GetStatusAttribute(StatusDataTable->GetStatusType());
		if (StatusAttribute.IsValid() == false)
		{
			continue;
		}

		FGameplayModifierInfo StatusModifierInfo;
		StatusModifierInfo.Attribute = StatusAttribute;
		StatusModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(StatusDataTable->GetValue()));

		EquipmentStatusEffect->Modifiers.Add(StatusModifierInfo);
	}

	FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(EquipmentStatusEffect);
	if (ActiveEffectHandle.IsValid() == false)
	{
		return;
	}

	ActiveEquipStatusEffectHandles.Add(EquipmentDataTable->GetEquipSlotType(), ActiveEffectHandle);
}

void UPNStatusActorComponent::UnApplyStatusFromEquipment(const EEquipSlotType EquipSlot)
{
	APNCharacter* Owner = GetOwner<APNCharacter>();
	check(Owner);

	UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	FActiveGameplayEffectHandle& ActiveEquipStatusEffectHandle = ActiveEquipStatusEffectHandles[EquipSlot];
	AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEquipStatusEffectHandle);
	ActiveEquipStatusEffectHandle.Invalidate();
	ActiveEquipStatusEffectHandles.Remove(EquipSlot);
}

void UPNStatusActorComponent::RequestHeal(const float HealAmount)
{
	if (HealAmount <= 0.0f)
	{
		return;
	}

	IPNAbilitySystemInterface* OwnerAbilitySystemInterface = Cast<IPNAbilitySystemInterface>(GetOwner());
	check(OwnerAbilitySystemInterface);

	UPNAbilitySystemComponent* AbilitySystemComponent = Cast<UPNAbilitySystemComponent>(OwnerAbilitySystemInterface->GetAbilitySystemComponent());
	check(AbilitySystemComponent);

	UGameplayEffect* HealEffect = NewObject<UGameplayEffect>(this, FName(TEXT("HealEffect")));
	HealEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo StatusModifierInfo;
	StatusModifierInfo.Attribute = UPNPawnAttributeSet::GetHealAttribute();
	StatusModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(HealAmount));
	HealEffect->Modifiers.Add(StatusModifierInfo);

	AbilitySystemComponent->ApplyGameplayEffectToSelf(HealEffect);
}

bool UPNStatusActorComponent::IsDead() const
{
	IPNAbilitySystemInterface* AbilitySystemInterface = GetOwner<IPNAbilitySystemInterface>();
	if (UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
	{
		return AbilitySystemComponent->HasMatchingGameplayTag(FPNGameplayTags::Get().Status_Dead);
	}

	return false;
}

UPNStatusActorComponent::UPNStatusActorComponent()
{
	bWantsInitializeComponent = true;
}

void UPNStatusActorComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (IPNAbilitySystemInterface* AbilitySystemInterface = GetOwner<IPNAbilitySystemInterface>())
	{
		AbilitySystemInterface->OnInitializeAbilitySystemDelegate.AddUObject(this, &ThisClass::OnInitializeAbilitySystem);
	}
}

void UPNStatusActorComponent::OnInitializeAbilitySystem()
{
	AActor* Owner = GetOwner();
	APNCharacter* OwnerCast = Cast<APNCharacter>(Owner);

	if (Owner->HasAuthority())
	{
		UAbilitySystemComponent* AbilitySystemComponent = GetOwner<IPNAbilitySystemInterface>()->GetAbilitySystemComponent();
		check(AbilitySystemComponent);

		// Todo. 데이터테이블과 연동해야 함	
		if (OwnerCast && OwnerCast->GetController() && OwnerCast->GetController()->IsPlayerController())
		{
			AbilitySystemComponent->InitStats(UPNPlayerAttributeSet::StaticClass(), nullptr);
		}
		else
		{
			AbilitySystemComponent->InitStats(UPNPawnAttributeSet::StaticClass(), nullptr);
		}


		const UPNPawnAttributeSet* PawnAttributeSet = AbilitySystemComponent->GetSet<UPNPawnAttributeSet>();
		PawnAttributeSet->OnOutOfHp.AddUObject(this, &ThisClass::OnOutOfHp);
		PawnAttributeSet->OnDamagedDelegate.AddUObject(this, &ThisClass::OnDamaged);

		AbilitySystemComponent->AddLooseGameplayTag(FPNGameplayTags::Get().Status_Peace);
		FGameplayEventData PayLoad;
		AbilitySystemComponent->HandleGameplayEvent(FPNGameplayTags::Get().Status_Peace, &PayLoad);

		// Todo. 추후 Pawn을 구현한다면 OnInitializedStatus를 Interface로 선언해야 함
		if (OwnerCast)
		{
			OwnerCast->OnInitializedStatus();
		}

		AbilitySystemComponent->RegisterGameplayTagEvent(FPNGameplayTags::Get().Action_Attack, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnActionTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(FPNGameplayTags::Get().Action_Guard, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnActionTagChanged);
	}

	if (OwnerCast->GetController() && OwnerCast->GetController()->IsLocalController())
	{
		Owner->FindComponentByClass<UPNDetectComponent>()->OnDetectedDelegate.AddUObject(this, &ThisClass::OnDetected);
	}
}

FGameplayAttribute UPNStatusActorComponent::GetStatusAttribute(const EStatusType StatusType) const
{
	switch (StatusType)
	{
	case EStatusType::Power:
		{
			return UPNPawnAttributeSet::GetPowerAttribute();
		}
	default:
		{
			break;
		}
	}

	return FGameplayAttribute();
}

EStatusType UPNStatusActorComponent::GetStatusType(const FGameplayAttribute Attribute) const
{
	APNCharacter* Owner = GetOwner<APNCharacter>();
	check(Owner);

	if (Attribute == UPNPawnAttributeSet::GetHpAttribute() || Attribute == UPNPawnAttributeSet::GetDamageAttribute())
	{
		return EStatusType::Hp;
	}
	else if (Attribute == UPNPawnAttributeSet::GetMaxHpAttribute())
	{
		return EStatusType::MaxHp;
	}

	return EStatusType::Invalid;
}

void UPNStatusActorComponent::OnOutOfHp() const
{
	APNCharacter* Owner = GetOwner<APNCharacter>();
	check(Owner);

	UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	AbilitySystemComponent->AddLooseGameplayTag(FPNGameplayTags::Get().Status_Dead, 1);

	Owner->SetDead();
}

void UPNStatusActorComponent::OnDamaged()
{
	SetPeaceOrFightStatus(FPNGameplayTags::Get().Status_Fight);
}

void UPNStatusActorComponent::SetPeaceOrFightStatus(const FGameplayTag StatusTag)
{
	if (StatusTag.MatchesTag(FPNGameplayTags::Get().Status_Fight) == false && StatusTag.MatchesTag(FPNGameplayTags::Get().Status_Peace) == false)
	{
		ensure(false);
		return;
	}

	if (IPNAbilitySystemInterface* OwnerAbilitySystemInterface = GetOwner<IPNAbilitySystemInterface>())
	{
		UAbilitySystemComponent* AbilitySystemComponent = OwnerAbilitySystemInterface->GetAbilitySystemComponent();
		check(AbilitySystemComponent);

		AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Status_Fight, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(FPNGameplayTags::Get().Status_Peace, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(StatusTag, 1);
	}

	if (StatusTag.MatchesTag(FPNGameplayTags::Get().Status_Fight) && CheckTransitionToPeaceTimerHandle.IsValid() == false)
	{
		GetWorld()->GetTimerManager().SetTimer(CheckTransitionToPeaceTimerHandle, this, &ThisClass::CheckTransitionToPeaceTimerCallback, CheckDetectEnemyPeriod, true, CheckDetectEnemyPeriod);
	}
}

void UPNStatusActorComponent::CheckTransitionToPeaceTimerCallback()
{
	const AActor* DetectedEnemy = GetOwner()->FindComponentByClass<UPNDetectComponent>()->GetDetectedEnemy();
	if (IsValid(DetectedEnemy))
	{
		NoEnemyDetectTime = 0.0f;
	}
	else
	{
		NoEnemyDetectTime += CheckDetectEnemyPeriod;
		if (NoDetectionEnemyToPeaceTime <= NoEnemyDetectTime)
		{
			SetPeaceOrFightStatus(FPNGameplayTags::Get().Status_Peace);
			GetWorld()->GetTimerManager().ClearTimer(CheckTransitionToPeaceTimerHandle);
		}
	}
}

void UPNStatusActorComponent::OnDetected()
{
	SetPeaceOrFightStatus(FPNGameplayTags::Get().Status_Fight);
}

void UPNStatusActorComponent::OnActionTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if ((Tag.MatchesTag(FPNGameplayTags::Get().Action_Attack) && NewCount > 0) || (Tag.MatchesTag(FPNGameplayTags::Get().Action_Guard) && NewCount > 0))
	{
		SetPeaceOrFightStatus(FPNGameplayTags::Get().Status_Fight);
	}
}
