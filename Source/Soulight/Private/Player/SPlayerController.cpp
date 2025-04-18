// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayerController.h"

#include "Player/SPlayer.h"
#include "Player/Abilities/SAbilityDataBase.h"

#include "Kismet/GameplayStatics.h"

#include "Widgets/SGameplayUI.h"
#include "Widgets/SPlayerHUDUI.h"
#include "Widgets/SStatsUI.h"

void ASPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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
		PlayerHUDUI->AddToViewport();
	}
	else
	{
		GameplayUI->AddToViewport();
		PlayerHUDUI->RemoveFromParent();
	}
}

void ASPlayerController::AddAbility(USAbilityDataBase* newAbility, EUpgrade upgrade)
{
	PlayerHUDUI->StatsUI->AddAbility(newAbility, upgrade);

	switch (newAbility->GetType())
	{
		case EType::Skill:
			GameplayUI->SetSkillImage(newAbility->GetAbilityIcon());
			break;
		case EType::Spell:
			GameplayUI->SetSpellImage(newAbility->GetAbilityIcon());
			break;
		default:

			break;
	}
}

void ASPlayerController::SetStatsUI(float Str, float Def, float Agi, float Soul)
{
	if (!IsValid(PlayerHUDUI)) return;

	PlayerHUDUI->SetStats(Str, Def, Agi, Soul);
}

void ASPlayerController::SetHealthUI(float Current, float Max)
{
	if (!IsValid(GameplayUI)) return;

	GameplayUI->SetHealth(Current, Max);
}

void ASPlayerController::SetGameplayUIVisibility(bool bShowGameplayUI)
{
	GameplayUI->SetVisibility(bShowGameplayUI ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
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

	PlayerHUDUI = CreateWidget<USPlayerHUDUI>(this, PlayerHUDUIWidgetClass);

	if (PlayerHUDUI == nullptr)
	{
		return;
	}

	GameplayUI->InitGameplayUI(PlayerCharacter);

	GameplayUI->AddToViewport();
	PlayerHUDUI->AddToViewport();
	PlayerHUDUI->RemoveFromParent(); //hides it, useful for certain buttons dealing with UI
}
