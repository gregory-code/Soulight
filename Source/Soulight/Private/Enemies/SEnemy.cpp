// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Engine/World.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

ASEnemy::ASEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AI Perception Source Comp");
	AIPerceptionSourceComp->RegisterForSense(UAISense_Sight::StaticClass());
	AIPerceptionSourceComp->RegisterWithPerceptionSystem();
}
