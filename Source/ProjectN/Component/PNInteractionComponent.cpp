// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PNInteractionComponent.h"

#include "UI/PNHUD.h"

bool UPNInteractionComponent::CanInteraction() const
{
	// Todo. Interaction 어빌리티가 가능한지 체크해야 함

	return true;
}

bool UPNInteractionComponent::GetInteractionOption(FInteractionOption& InInteractionOption)
{
	if (!CanInteraction())
	{
		return false;
	}
	
	InInteractionOption.InteractionDataTableKey = InteractionDataTableKey;
	InInteractionOption.InteractionTargetActorKey = GetOwner();
	
	return true;
}