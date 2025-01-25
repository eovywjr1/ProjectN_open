// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PNAssetManager.h"
#include "PNGameplayTags.h"

void UPNAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FPNGameplayTags::InitializeNativeTags();
}
