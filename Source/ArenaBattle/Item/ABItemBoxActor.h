// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItemBoxActor.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBoxActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AABItemBoxActor();

	virtual void BeginPlay() override;

	FORCEINLINE class UBoxComponent* GetTrigger() const { return Trigger; }

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEffectFinished(UParticleSystemComponent* PSystem);

private:
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UParticleSystemComponent> Effect;

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemData> Item;
};
