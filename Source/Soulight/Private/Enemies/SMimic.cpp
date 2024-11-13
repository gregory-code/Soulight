// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SMimic.h"

#include "Engine/World.h"

#include "Framework/SInteractableObject.h"

void ASMimic::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle EscapeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EscapeTimerHandle, this, &ASMimic::Escape, 1.0f, false, EscapeDelay);

	LastSound = 0.0f;
}

void ASMimic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LastSound++;
	if (LastSound > SoundIncrement)
	{
		MakeSound();
		LastSound = 0.0f;
	}
}

void ASMimic::StartDeath(bool IsDead)
{
	UE_LOG(LogTemp, Warning, TEXT("I Am Doing this"));

	if (LootPool.IsEmpty() == false) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Loot Table Is Not Empty"));

		const int32 rand = FMath::RandRange(0, LootPool.Num() - 1);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASInteractableObject* Equipment = GetWorld()->SpawnActor<ASInteractableObject>(LootPool[rand], GetActorLocation(), GetActorRotation(), SpawnParams);
	}
	
	Destroy();
}

void ASMimic::Escape()
{
	// Maybe do other things here idk

	Destroy();
}
		