// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SStatsUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Player/Abilities/SAbilityDataBase.h"

void USStatsUI::AddAbility(USAbilityDataBase* newSkill/*, EUpgrade upgradeType*/)
{
	switch (newSkill->GetType())
	{
		case EType::Skill:
		ApplyAbilityToType(newSkill, SkillName, SkillDescription, SkillImage);
		break;

		case EType::Spell:
		ApplyAbilityToType(newSkill, SpellName, SpellDescription, SpellImage);
		break;

		case EType::Passive:
		ApplyAbilityToType(newSkill, PassiveName, PassiveDescription, PassiveImage);
		break;
	}
}

void USStatsUI::ApplyAbilityToType(USAbilityDataBase* newSkill, UTextBlock* targetName, UTextBlock* targetDescription, UImage* targetImage)
{
	targetName->SetText(FText::FromString(newSkill->GetAbilityName()));
	targetDescription->SetText(FText::FromString(newSkill->GetAbilityDescription()));
	targetImage->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParameterName, newSkill->GetAbilityIcon());
}
