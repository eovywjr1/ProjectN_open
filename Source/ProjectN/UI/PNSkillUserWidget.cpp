// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PNSkillUserWidget.h"

#include "Component/PNSkillComponent.h"
#include "Components/Image.h"

void UPNSkillUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// if (UPNSkillComponent* SkillComponent = GetOwningPlayerPawn()->FindComponentByClass<UPNSkillComponent>())
	// {
	// 	if (SkillComponent->IsEnableSkill(SkillInputTag))
	// 	{
	// 		SkillImage->SetOpacity(1.0f);
	// 	}
	// 	else
	// 	{
	// 		SkillImage->SetOpacity(0.3f);
	// 	}
	// }
}
