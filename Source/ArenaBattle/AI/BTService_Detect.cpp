// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"

#include "ABAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"
#include "Physics/ABCollision.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	check(Owner);

	IABCharacterAIInterface* OwnerAIInterface = Cast<IABCharacterAIInterface>(Owner);
	check(OwnerAIInterface);

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Detect), false, Owner);
	bool bResult = Owner->GetWorld()->OverlapMultiByChannel(Overlaps, Owner->GetActorLocation(), FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(OwnerAIInterface->GetAIDetectRange()), Params);
	if (bResult)
	{
		for (const FOverlapResult& OverlapResult : Overlaps)
		{
			APawn* DetectedPawn = Cast<APawn>(OverlapResult.GetActor());
			if (DetectedPawn && DetectedPawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, DetectedPawn);

#ifdef ENABLE_DRAW_DEBUG
				DrawDebugSphere(Owner->GetWorld(), Owner->GetActorLocation(), OwnerAIInterface->GetAIDetectRange(), 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(Owner->GetWorld(), DetectedPawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(Owner->GetWorld(), Owner->GetActorLocation(), DetectedPawn->GetActorLocation(), FColor::Green, false, 0.27f);
#endif

				return;
			}
		}
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

#ifdef ENABLE_DRAW_DEBUG
	DrawDebugSphere(Owner->GetWorld(), Owner->GetActorLocation(), OwnerAIInterface->GetAIDetectRange(), 16, FColor::Red, false, 0.2f);
#endif
}
