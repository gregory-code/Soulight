// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SMimic.h"

#include "Engine/World.h"

#include "Framework/SInteractableObject.h"

void ASMimic::StartDeath(bool IsDead)
{
	float RNG = FMath::RandRange(0, 100);

	if (RNG > 50)
	{
		if (LootPool.IsEmpty()) return;

		const int32 rand = FMath::RandRange(0, LootPool.Num() - 1);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASInteractableObject* Ability = GetWorld()->SpawnActor<ASInteractableObject>(LootPool[rand], GetActorLocation(), GetActorRotation(), SpawnParams);
	}
	else
	{
		if (EquipmentLootPool.IsEmpty()) return;

		const int32 rand = FMath::RandRange(0, EquipmentLootPool.Num() - 1);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASInteractableObject* Equipment = GetWorld()->SpawnActor<ASInteractableObject>(EquipmentLootPool[rand], GetActorLocation(), GetActorRotation(), SpawnParams);
	}
	
}
