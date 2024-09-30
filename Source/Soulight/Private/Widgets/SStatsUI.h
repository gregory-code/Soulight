// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SStatsUI.generated.h"

/**
 * 
 */
UCLASS()
class USStatsUI : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PassiveDescription;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PassiveName;

	UPROPERTY(meta = (BindWidget))
	class UImage* PassiveImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillDescription;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SpellDescription;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SpellName;

	UPROPERTY(meta = (BindWidget))
	class UImage* SpellImage;
};
