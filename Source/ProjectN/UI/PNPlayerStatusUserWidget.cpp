// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PNPlayerStatusUserWidget.h"

#include "PNHUD.h"
#include "PNLogChannels.h"
#include "AbilitySystem/AttributeSet/PNPlayerAttributeSet.h"
#include "Actor/PNCharacterPlayer.h"
#include "Components/ProgressBar.h"
#include "DataTable/StatusDataTable.h"

void UPNPlayerStatusUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APNHUD* HUD = Cast<APNHUD>(GetOwningPlayer()->GetHUD());
	check(HUD);

	HUD->OnStatusChangedDelegate.AddUObject(this, &ThisClass::OnStatusChanged);
	HUD->OnInitStatusDelegate.AddUObject(this, &ThisClass::OnInitStatus);

	if (StatusType == EStatusType::Hp)
	{
		ProgressBar = Cast<UProgressBar>(GetWidgetFromName("StatusProgressBar"));
		if (ProgressBar == nullptr)
		{
			UE_LOG(LogPN, Error, TEXT("StatusWidget에 StatusType이 %s인데 StatusProgressBar가 없습니다."), *StaticEnum<EStatusType>()->GetNameStringByValue(static_cast<int64>(StatusType)));
			return;
		}
	}
}

void UPNPlayerStatusUserWidget::OnStatusChanged(const FObjectKey TargetObjectKey, const EStatusType InStatusType)
{
	if (StatusType != InStatusType)
	{
		return;
	}

	UpdateStatus(TargetObjectKey);
}

void UPNPlayerStatusUserWidget::OnInitStatus(const FObjectKey TargetObjectKey)
{
	UpdateStatus(TargetObjectKey);
}

void UPNPlayerStatusUserWidget::UpdateStatus(const FObjectKey TargetObjectKey) const
{
	APNCharacter* TargetActor = Cast<APNCharacter>(TargetObjectKey.ResolveObjectPtr());
	if (TargetActor == nullptr || TargetActor->IsPlayer() == false)
	{
		return;
	}
	
	UAbilitySystemComponent* AbilitySystemComponent = TargetActor->GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	const UPNPlayerAttributeSet* PlayerAttributeSet = AbilitySystemComponent->GetSet<UPNPlayerAttributeSet>();
	if (PlayerAttributeSet == nullptr)
	{
		return;
	}

	switch (StatusType)
	{
	case EStatusType::Hp:
	case EStatusType::MaxHp:
		{
			if (ProgressBar == nullptr)
			{
				return;
			}

			ProgressBar->SetPercent(PlayerAttributeSet->GetHp() / PlayerAttributeSet->GetMaxHp());
		}
	default:
		{
			break;
		}
	}
}
