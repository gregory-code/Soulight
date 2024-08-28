// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayerController.h"
#include "Player/SPlayer.h"

void ASPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	PostPossessionSetup(NewPawn);
	if (PlayerCharacter)
	{
		
	}
}

void ASPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);
	if (!HasAuthority())
	{
		PostPossessionSetup(NewPawn);
	}
}

void ASPlayerController::PostPossessionSetup(APawn* NewPawn)
{
	PlayerCharacter = Cast<ASPlayer>(NewPawn);
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Needs SPlayerCharacter as Pawn"));
		return;
	}

	SpawnGameplayUI();
}

void ASPlayerController::SpawnGameplayUI()
{
	if (!PlayerCharacter)
	{
		return;
	}
}
