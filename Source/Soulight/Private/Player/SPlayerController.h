// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

/**
 * 
 */

class USAbilityDataBase;

UCLASS()
class ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* NewPawn) override;

	virtual void AcknowledgePossession(APawn* NewPawn) override;

	void GameplayUIState(bool state);

	UFUNCTION()
	void AddAbility(USAbilityDataBase* newAbility, EUpgrade upgrade);

private:
	void PostPossessionSetup(APawn* NewPawn);

	void SpawnGameplayUI();

	UPROPERTY()
	class ASPlayer* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USGameplayUI> GameplayUIWidgetClass;

	UPROPERTY()
	class USGameplayUI* GameplayUI;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USPlayerHUDUI> PlayerHUDUIWidgetClass;

	UPROPERTY()
	class USPlayerHUDUI* PlayerHUDUI;
};
