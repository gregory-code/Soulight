// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SStatsUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Player/Abilities/SAbilityDataBase.h"

void USStatsUI::AddAbility(USAbilityDataBase* newSkill, EUpgrade upgradeType)
{
	UTextBlock* targetName = nullptr;
	UTextBlock* targetDescription = nullptr;
	UImage* targetImage = nullptr;
	UImage* targetStar1 = nullptr;
	UImage* targetStar2 = nullptr;
	UImage* targetStar3 = nullptr;

	switch (newSkill->GetType())
	{
		case EType::Skill:
			targetName = SkillName;
			targetDescription = SkillDescription;
			targetImage = SkillImage;
			targetStar1 = SkillStar1;
			targetStar1 = SkillStar2;
			targetStar1 = SkillStar3;
		break;

		case EType::Spell:
			targetName = SpellName;
			targetDescription = SpellDescription;
			targetImage = SpellImage;
			targetStar1 = SpellStar1;
			targetStar1 = SpellStar2;
			targetStar1 = SpellStar3;
		break;

		case EType::Passive:
			targetName = PassiveName;
			targetDescription = PassiveDescription;
			targetImage = PassiveImage;
			targetStar1 = passiveStar1;
			targetStar1 = PassiveStar2;
			targetStar1 = PassiveStar3;
		break;
	}
	
	if (targetName == nullptr)
		return;

	if (upgradeType == EUpgrade::Upgrade)
	{

	}
	else
	{
		ApplyNewSkill(newSkill, targetName, targetDescription, targetImage, targetStar1, targetStar2, targetStar3);
	}
}

void USStatsUI::ApplyNewSkill(USAbilityDataBase* newSkill, UTextBlock*& targetName, UTextBlock*& targetDescription, UImage*& targetImage, class UImage*& star1, class UImage*& star2, class UImage*& star3)
{
	targetName->SetText(FText::FromString(newSkill->GetAbilityName()));
	targetDescription->SetText(FText::FromString(newSkill->GetAbilityDescription()));
	targetImage->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParameterName, newSkill->GetAbilityIcon());
	
	star1->SetBrushFromTexture(FullStarTexture);
	star2->SetBrushFromTexture(EmptyStarTexture);
	star3->SetBrushFromTexture(EmptyStarTexture);
}
