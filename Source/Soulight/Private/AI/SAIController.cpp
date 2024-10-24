// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

#include "Player/SPlayer.h"

ASAIController::ASAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");

	SightConfig->PeripheralVisionAngleDegrees = 60.f;

	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = 600.f;

	SightConfig->SetMaxAge(5.f);
	SightConfig->PointOfViewBackwardOffset = 50.0f;

	// Set all to true for it to detect player, if this becomes an issue in the future blame joseph
	//	- Allen
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
}

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	if(GetBrainComponent())
		GetBrainComponent()->StartLogic();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASAIController::OnTargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ASAIController::OnTargetForgotten);

	AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
	AIPerceptionComponent->Activate(true);
}

void ASAIController::Tick(float DeltaTime)
{
	/*
	if (IsValid(Player))
	{
		FVector const PlayerLocation = Player->GetActorLocation();

		GetBlackboardComponent()->SetValueAsVector(TargetBBKeyName, PlayerLocation);
	}
	*/
}

void ASAIController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (GetCharacter() && GetCharacter()->GetMesh())
	{
		OutLocation = GetCharacter()->GetMesh()->GetSocketLocation(AIVisionAttachSocketName);
		OutRotation = GetCharacter()->GetMesh()->GetSocketRotation(AIVisionAttachSocketName);
		OutRotation.Pitch = 0;
	}
	else
	{
		Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	}
}

void ASAIController::SenseSightStimulus(AActor* Target, FAIStimulus Stimulus)
{
	if (Stimulus.Type != UAISense::GetSenseID<UAISense_Sight>()) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(PlayerBBKeyName, Target);
	}
	else
	{
		GetBlackboardComponent()->ClearValue(PlayerBBKeyName);
	}
}

void ASAIController::SenseHearingStimulus(AActor* Target, FAIStimulus Stimulus)
{
	if (Stimulus.Type != UAISense::GetSenseID<UAISense_Hearing>()) return;

	UE_LOG(LogTemp, Warning, TEXT("AI Hearing Perceived!"));

	if (Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsVector(LocationBBKeyName, Stimulus.StimulusLocation);
		UE_LOG(LogTemp, Warning, TEXT("Hearing Successful!"));

	}
	else
	{
		GetBlackboardComponent()->ClearValue(LocationBBKeyName);
		UE_LOG(LogTemp, Warning, TEXT("Hearing Failed!"));
	}
}

void ASAIController::OnTargetPerceptionUpdated(AActor* Target, FAIStimulus Stimulus)
{
	if (IsValid(Target) == false)
	{
		return;
	}

	if (!GetBlackboardComponent())
	{
		return;
	}

	SenseHearingStimulus(Target, Stimulus);

	if (!Target->IsA(ASPlayer::StaticClass())) return;

	SenseSightStimulus(Target, Stimulus);
}

void ASAIController::OnTargetForgotten(AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("I Forgor!"));

	AActor* CurrentTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(PlayerBBKeyName));
	if (CurrentTarget == Target)
	{
		GetBlackboardComponent()->ClearValue(PlayerBBKeyName);
		return;
	}
	else
	{
		GetBlackboardComponent()->ClearValue(LocationBBKeyName);
	}

}