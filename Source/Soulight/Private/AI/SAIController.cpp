// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ASAIController::ASAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");

	SightConfig->PeripheralVisionAngleDegrees = 360.0f;
	SightConfig->SightRadius = 500.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
}

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	AIPerceptionComponent->Activate(true);

	GetBrainComponent()->StartLogic();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ASAIController::Tick(float DeltaTime)
{
	if (IsValid(Player))
	{
		FVector const PlayerLocation = Player->GetActorLocation();

		GetBlackboardComponent()->SetValueAsVector(TargetBBKeyName, PlayerLocation);
	}
}
