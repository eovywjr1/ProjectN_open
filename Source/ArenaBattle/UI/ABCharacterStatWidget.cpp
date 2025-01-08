// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"

#include "Components/TextBlock.h"

void UABCharacterStatWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropertyIterator(FABCharacterStat::StaticStruct()); PropertyIterator; ++PropertyIterator)
	{
		float BaseData = 0.0f;
		PropertyIterator->GetValue_InContainer(&BaseStat, &BaseData);

		const FString PropertyKey(PropertyIterator->GetName());
		if (UTextBlock** BaseTextBlockPtr = BaseLookup.Find(*PropertyKey))
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		float ModifierData = 0.0f;
		PropertyIterator->GetValue_InContainer(&ModifierStat, &ModifierData);

		if (UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(*PropertyKey))
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropertyIterator(FABCharacterStat::StaticStruct()); PropertyIterator; ++PropertyIterator)
	{
		const FString PropertyKey(PropertyIterator->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropertyKey);
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropertyKey);

		if (UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName)))
		{
			BaseLookup.Add(*PropertyKey, BaseTextBlock);
		}
		if (UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName)))
		{
			ModifierLookup.Add(*PropertyKey, ModifierTextBlock);
		}
	}
}
