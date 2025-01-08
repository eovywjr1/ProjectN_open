// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBoxActor.h"

#include "ABItemData.h"
#include "ShaderPrintParameters.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/Character.h"
#include "Interface/ABCharacterItemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"

// Sets default values
AABItemBoxActor::AABItemBoxActor()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT(
		"/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT(
		"/Script/Engine.ParticleSystem'/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object); // 이펙트 지정
		Effect->bAutoActivate = false; // 바로 이펙트가 발동하지 않도록 설정
	}
}

void AABItemBoxActor::BeginPlay()
{
	Super::BeginPlay();

	if (Trigger)
	{
		Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f)); // 트리거 박스 사이즈 지정
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBoxActor::OnOverlapBegin); // 트리거 바인딩
	}
}

void AABItemBoxActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Item == nullptr)
	{
		Destroy();
		return;
	}

	if (IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor))
	{
		OverlappingPawn->TakeItem(Item);
	}

	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &AABItemBoxActor::OnEffectFinished); // 이펙트가 종료되면 호출되는 델리게이트
}

void AABItemBoxActor::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}

void AABItemBoxActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAssetManager& AssetManager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	AssetManager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);
	ensure(Assets.Num() > 0);

	const int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}

	Item = Cast<UABItemData>(AssetPtr.Get());
	ensure(Item);
}
