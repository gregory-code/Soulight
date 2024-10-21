// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

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

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASAIController::OnTargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ASAIController::OnTargetForgotten);

	AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
	AIPerceptionComponent->Activate(true);

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ASAIController::Tick(float DeltaTime)
{
	return;

	if (IsValid(Player))
	{
		FVector const PlayerLocation = Player->GetActorLocation();

		GetBlackboardComponent()->SetValueAsVector(TargetBBKeyName, PlayerLocation);
	}
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

	if (!Target->IsA(ASPlayer::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Joseph"));
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetBBKeyName, Target);
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TargetBBKeyName);
	}
}

void ASAIController::OnTargetForgotten(AActor* Target)
{
	AActor* CurrentTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetBBKeyName));
	if (CurrentTarget == Target)
	{
		TArray<AActor*> OtherTargets;
		AIPerceptionComponent->GetPerceivedHostileActors(OtherTargets);
		if (OtherTargets.Num() != 0)
		{
			GetBlackboardComponent()->SetValueAsObject(TargetBBKeyName, OtherTargets[0]);
		}
		else
		{
			GetBlackboardComponent()->ClearValue(TargetBBKeyName);
		}
	}
}