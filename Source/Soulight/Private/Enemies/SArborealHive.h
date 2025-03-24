// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/SEnemy.h"
#include "SArborealHive.generated.h"

/**
 * 
 */
UCLASS()
class ASArborealHive : public ASEnemy
{
	GENERATED_BODY()

public:
	ASArborealHive();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	class USphereComponent* RangeCollisionSphere;

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<ASEnemy> EnemyClass;

	TArray<AActor*> InstancedEnemies;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int SpawnLimit = 3;

	bool bCanSpawn = false;
	void SpawnEnemy();
	UFUNCTION()
	void OnEnemyDead(bool IsDead, AActor* DeadActor);

	FTimerHandle SpawnDelayHandle;
	UFUNCTION()
	void SpawnDelay();
	bool bSpawnDelayActive = false;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float DelayDuration = 3.f;
};
