// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABStageGimmick.h"

#include "Components/BoxComponent.h"
#include "Item/ABItemBoxActor.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"

// Sets default values
AABStageGimmick::AABStageGimmick()
{
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	// Gate Section
	static FName GateSockets[] = {TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate")};
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));
	if (GateMeshRef.Object)
	{
		for (FName GateSocket : GateSockets)
		{
			UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
			Gate->SetStaticMesh(GateMeshRef.Object);
			Gate->SetupAttachment(Stage, GateSocket); // 소켓을 지정하여 레벨디자인을 편리하게 조정 가능
			Gate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
			Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
			Gates.Add(GateSocket, Gate);

			FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
			UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
			GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
			GateTrigger->SetupAttachment(Stage, GateSocket);
			GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
			GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
			GateTrigger->ComponentTags.Add(GateSocket);
			GateTriggers.Add(GateTrigger);
		}
	}

	// Fight Section
	OpponentClass = AABCharacterNonPlayer::StaticClass();

	// Reward Section
	RewardBoxClass = AABItemBoxActor::StaticClass();

	for (const FName& GateSocket : GateSockets)
	{
		const FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}
}

void AABStageGimmick::BeginPlay()
{
	Super::BeginPlay();

	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnStageTriggerBeginOverlap);

	for (TObjectPtr<UBoxComponent>& GateTrigger : GateTriggers)
	{
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);
	}
}

#ifdef WITH_EDITOR
void AABStageGimmick::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetState(CurrentState);
}
#endif

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(OverlappedComponent->ComponentTags.Num() == 1);

	const FName ComponentTag = OverlappedComponent->ComponentTags[0];
	const FName SocketName = FName(*ComponentTag.ToString().Left(2)); // 컴포넌트 태그 앞 두 글자
	check(Stage->DoesSocketExist(SocketName));

	// 스테이지가 설치되어 있는지 검사
	const FVector NewLocation = Stage->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(GateTrigger), false, this);
	bool bResult = GetWorld()->OverlapMultiByObjectType(OverlapResults, NewLocation, FQuat::Identity, FCollisionObjectQueryParams::InitType::AllStaticObjects, FCollisionShape::MakeSphere(775.0f), CollisionParam); // 해당 위치에 배치되어 있는지 검사
	if (bResult == false)
	{
		if (AABStageGimmick* NewGimmick = GetWorld()->SpawnActor<AABStageGimmick>(NewLocation, FRotator::ZeroRotator))
		{
			NewGimmick->SetStageNum(CurrentStageNum + 1);
		}
	}
}

void AABStageGimmick::OpenAllGate()
{
	for (auto& Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGate()
{
	for (auto& Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;

	switch (CurrentState)
	{
	case EStageState::READY:
		SetReady();
		break;

	case EStageState::FIGHT:
		SetFight();
		break;

	case EStageState::REWARD:
		SetChooseReward();
		break;

	case EStageState::NEXT:
		SetChooseNext();
		break;
	}
}

void AABStageGimmick::SetReady()
{
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	for (auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	OpenAllGate();
}

void AABStageGimmick::SetFight()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	for (auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGate();

	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false);
}

void AABStageGimmick::SetChooseReward()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	for (auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGate();
	SpawnRewardBoxes();
}

void AABStageGimmick::SetChooseNext()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	for (auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}

	OpenAllGate();
}

void AABStageGimmick::OnOpponentDestroyed(AActor* DestoryedActor)
{
	SetState(EStageState::REWARD);
}

void AABStageGimmick::OnOpponentSpawn()
{
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.0f);

	if (AABCharacterNonPlayer* ABOpponentCharacter = GetWorld()->SpawnActorDeferred<AABCharacterNonPlayer>(OpponentClass, SpawnTransform))
	{
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnOpponentDestroyed);
		ABOpponentCharacter->SetCurrentLevel(CurrentStageNum);
		ABOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}

void AABStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid() == false)
		{
			continue;
		}

		// 오버랩된 아이템 박스는 자동으로 동작하므로 오버랩되지 않은 아이템 박스만 소멸시킴
		AABItemBoxActor* ValidItemBox = RewardBox.Get();
		if (ValidItemBox != OverlappedComponent->GetOwner())
		{
			ValidItemBox->Destroy();
		}
	}

	SetState(EStageState::NEXT);
}

void AABStageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		const FVector WorldSpawnLocation = GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f, 0.0f, 30.0f);

		if (AABItemBoxActor* RewardBoxActor = Cast<AABItemBoxActor>(GetWorld()->SpawnActor(RewardBoxClass, &WorldSpawnLocation, &FRotator::ZeroRotator)))
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnRewardTriggerBeginOverlap);
			RewardBoxes.Add(RewardBoxActor);
		}
	}
}
