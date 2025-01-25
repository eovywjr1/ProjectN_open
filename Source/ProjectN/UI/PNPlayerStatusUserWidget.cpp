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

void UPNPlayerStatusUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	IAbilitySystemInterface* TargetAbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetObjectKey.ResolveObjectPtr());
	if (TargetAbilitySystemInterface == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = TargetAbilitySystemInterface->GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}

	const UPNPawnAttributeSet* AttributeSet = AbilitySystemComponent->GetSet<UPNPawnAttributeSet>();
	if (AttributeSet == nullptr)
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

			ProgressBar->SetPercent(AttributeSet->GetHp() / AttributeSet->GetMaxHp());
		}
	default:
		{
			break;
		}
	}
}
