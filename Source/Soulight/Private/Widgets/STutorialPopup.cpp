// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/STutorialPopup.h"

#include "GameFramework/PlayerController.h"

#include "Player/SPlayer.h"

void USTutorialPopup::Init(ASPlayer* Player)
{
	if (!Player) return;

	OwningPlayer = Player;
}

void USTutorialPopup::EnablePlayerControls()
{
	if (!OwningPlayer) return;

	UE_LOG(LogTemp, Warning, TEXT("Enabling Player Controls"));

	OwningPlayer->SetPlayerInputEnabled(true);

	if (APlayerController* PlayerController = Cast<APlayerController>(OwningPlayer->GetController()))
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetPause(false); // Unpause game
	}
}
