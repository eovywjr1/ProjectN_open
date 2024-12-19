// Fill out your copyright notice in the Description page of Project Settings.


#include "PNStatusActorComponent.h"

#include "AbilitySystemComponent.h"
#include "Actor/PNCharacter.h"
#include "AbilitySystem/AttributeSet/PNPawnAttributeSet.h"
#include "AbilitySystem/AttributeSet/PNWeaponAttributeSet.h"

void UPNStatusActorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APNCharacter* Owner = GetOwner<APNCharacter>())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent())
		{
			// Todo. 임시로 생성자에서 설정, 추후 무기 장착/획득할 때 넣어야 함
			if (TSubclassOf<UPNWeaponAttributeSet> WeaponAttributeSetClass = LoadClass<UPNWeaponAttributeSet>(this, TEXT("/Script/Engine.Blueprint'/Game/ProjectN/Blueprints/AttributeSet/BP_BasicWeaponAttributeSet.BP_BasicWeaponAttributeSet_C'")))
			{
				if (UPNWeaponAttributeSet* WeaponAttributeSet = NewObject<UPNWeaponAttributeSet>(this, WeaponAttributeSetClass))
				{
					AbilitySystemComponent->AddSpawnedAttribute(WeaponAttributeSet);
				}
			}
			
			// Todo. 데이터테이블과 연동해야 함	
			AbilitySystemComponent->InitStats(UPNPawnAttributeSet::StaticClass(), nullptr);
		}
	}
}
