// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayerController.h"
#include "Player/SPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/SGameplayUI.h"

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

void ASPlayerController::GameplayUIState(bool state)
{
	UGameplayStatics::SetGamePaused(GetWorld(), state);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(state);
	GetWorld()->GetFirstPlayerController()->bEnableClickEvents = state;

	if (state)
	{
		GameplayUI->RemoveFromParent();
	}
	else
	{
		GameplayUI->AddToViewport();
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

	GameplayUI = CreateWidget<USGameplayUI>(this, GameplayUIWidgetClass);

	if (GameplayUI == nullptr)
	{
		return;
	}

	GameplayUI->AddToViewport();
	//GameplayUI->RemoveFromParent(); //hides it, useful for certain buttons dealing with UI
}
