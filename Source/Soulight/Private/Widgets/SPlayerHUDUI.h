// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPlayerHUDUI.generated.h"

/**
 * 
 */
UCLASS()
class USPlayerHUDUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class USStatsUI* StatsUI;

	UFUNCTION()
	void SetStats(float Str, float Def, float Agi, float Soul);
	
	UFUNCTION()
	void SetHealth(float Current, float Max);

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterCurrentHP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterMaxHP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterStrength;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterDefense;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterAgility;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterSoul;

	UPROPERTY(meta = (BindWidget))
	class UImage* LampHealthImage;

};
