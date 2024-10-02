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

public:

	void AddAbility(class USAbilityDataBase* newSkill);
	
private:

	void ApplyAbilityToType(class USAbilityDataBase* newSkill, class UTextBlock* targetName, class UTextBlock* targetDescription, class UImage* targetImage);

	UPROPERTY(EditDefaultsOnly)
	FName IconMaterialParameterName{ "Icon" };

	UPROPERTY(EditDefaultsOnly)
	UTexture* EmptyStarTexture;

	UPROPERTY(EditDefaultsOnly)
	UTexture* FullStarTexture;

	////////////////////////////////////

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

	///////////////////////////

	UPROPERTY(meta = (BindWidget))
	class UImage* passiveStar1;

	UPROPERTY(meta = (BindWidget))
	class UImage* PassiveStar2;

	UPROPERTY(meta = (BindWidget))
	class UImage* PassiveStar3;

	UPROPERTY(meta = (BindWidget))
	class UImage* SpellStar1;

	UPROPERTY(meta = (BindWidget))
	class UImage* SpellStar2;

	UPROPERTY(meta = (BindWidget))
	class UImage* SpellStar3;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillStar1;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillStar2;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillStar3;
};
