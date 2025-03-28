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
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* NewPawn) override;

	virtual void AcknowledgePossession(APawn* NewPawn) override;

	void GameplayUIState(bool state);

	UFUNCTION()
	void AddAbility(USAbilityDataBase* newAbility, EUpgrade upgrade);

	UFUNCTION()
	void SetStatsUI(float Str, float Def, float Agi, float Soul);

	UFUNCTION()
	void SetHealthUI(float Current, float Max);

	UFUNCTION(BlueprintCallable)
	USGameplayUI* GetGameplayUI() const { return GameplayUI; }

	UFUNCTION(BlueprintCallable)
	USPlayerHUDUI* GetPlayerHUD() const { return PlayerHUDUI; }

	UFUNCTION(BlueprintCallable)
	void SetGameplayUIVisibility(bool bShowGameplayUI);
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
