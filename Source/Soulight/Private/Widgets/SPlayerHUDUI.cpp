// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SPlayerHUDUI.h"
#include "Widgets/SStatsUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USPlayerHUDUI::SetStats(float Str, float Def, float Agi, float Soul)
{
	FText Text = FText::Format(FTextFormat::FromString("Strength: {0}"), FText::AsNumber(Str));
	CharacterStrength->SetText(Text);

	Text = FText::Format(FTextFormat::FromString("Defense: {0}"), FText::AsNumber(Def));
	CharacterDefense->SetText(Text);

	Text = FText::Format(FTextFormat::FromString("Agility: {0}"), FText::AsNumber(Agi));
	CharacterAgility->SetText(Text);

	Text = FText::Format(FTextFormat::FromString("Soul: {0}"), FText::AsNumber(Soul));
	CharacterSoul->SetText(Text);
}

void USPlayerHUDUI::SetHealth(float Current, float Max)
{
	if (LampHealthImage)
	{
		float Opacity = Current / Max;

		FLinearColor NewColor = LampHealthImage->Brush.TintColor.GetSpecifiedColor();
		NewColor.A = Opacity;  

		LampHealthImage->SetColorAndOpacity(NewColor);
	}
}
