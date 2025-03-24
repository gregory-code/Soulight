// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SArborealHive.h"

#include "Components/SphereComponent.h"

#include "Player/SPlayer.h"
#include "Enemies/SEnemy.h"

ASArborealHive::ASArborealHive()
{
	PrimaryActorTick.bCanEverTick = true;

	RangeCollisionSphere = CreateDefaultSubobject<USphereComponent>("Range Collision Sphere");
	RangeCollisionSphere->SetupAttachment(GetRootComponent());
}

void ASArborealHive::BeginPlay()
{
	Super::BeginPlay();

	RangeCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASArborealHive::OnBeginOverlap);
	RangeCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ASArborealHive::OnEndOverlap);
}

void ASArborealHive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InstancedEnemies.Num() == SpawnLimit) return;

	if (!bCanSpawn) return;

	if (!bSpawnDelayActive)
	{
		bSpawnDelayActive = true;

		SpawnEnemy();

		GetWorld()->GetTimerManager().SetTimer(SpawnDelayHandle, this, &ASArborealHive::SpawnDelay, 1/24.f, false, DelayDuration);
	}
}

void ASArborealHive::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPlayer* Player = Cast<ASPlayer>(OtherActor);
	if (!IsValid(Player)) return;

	UE_LOG(LogTemp, Warning, TEXT("Arboreal Hive Begin Overlap"));

	bCanSpawn = true;
}

void ASArborealHive::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASPlayer* Player = Cast<ASPlayer>(OtherActor);
	if (!IsValid(Player)) return;

	UE_LOG(LogTemp, Warning, TEXT("Arboreal Hive End Overlap"));

	bCanSpawn = false;
}

void ASArborealHive::SpawnEnemy()
{
	if (!IsValid(EnemyClass)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Class Is Null"));
		
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector SpawnLoc = GetActorLocation();

	ASEnemy* Enemy = GetWorld()->SpawnActor<ASEnemy>(EnemyClass, SpawnLoc, FRotator::ZeroRotator, SpawnParams);
	if (!IsValid(Enemy)) return;

	InstancedEnemies.Add(Enemy);

	Enemy->OnDead.AddDynamic(this, &ASArborealHive::OnEnemyDead);
}

void ASArborealHive::OnEnemyDead(bool IsDead, AActor* DeadActor)
{
	if (!IsValid(DeadActor)) return;

	InstancedEnemies.Remove(DeadActor);
}

void ASArborealHive::SpawnDelay()
{
	bSpawnDelayActive = false;
}
