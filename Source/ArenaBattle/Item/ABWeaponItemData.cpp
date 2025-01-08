// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABWeaponItemData.h"

USkeletalMesh* UABWeaponItemData::GetSkeletalMesh() const
{
	if (WeaponMesh.IsPending()) // 로딩이 안된 상태라면
	{
		WeaponMesh.LoadSynchronous(); // 동기적 로딩 실행
	}

	return WeaponMesh.Get();
}

UABWeaponItemData::UABWeaponItemData()
{
	Type = EItemType::Weapon;
}
