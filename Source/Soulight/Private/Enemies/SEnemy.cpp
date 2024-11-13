// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SEnemy.h"

#include "AIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Engine/World.h"

#include "Framework/SInteractableObject.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

ASEnemy::ASEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AI Perception Source Comp");
	AIPerceptionSourceComp->RegisterForSense(UAISense_Sight::StaticClass());
	AIPerceptionSourceComp->RegisterWithPerceptionSystem();

	OnDead.AddDynamic(this, &ASEnemy::StartDeath);
}

void ASEnemy::StartDeath(bool IsDead)
{
	float RNG = FMath::RandRange(0 , 100);

	if (LootPool.IsEmpty() == false && RNG > 50)
	{
		const int32 rand = FMath::RandRange(0, LootPool.Num() - 1);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASInteractableObject* Ability = GetWorld()->SpawnActor<ASInteractableObject>(LootPool[rand], GetActorLocation(), GetActorRotation(), SpawnParams);
	}

	Destroy();
}