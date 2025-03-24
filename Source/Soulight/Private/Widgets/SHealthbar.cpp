// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SHealthbar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USHealthbar::UpdateHealthbar(float Health, float MaxHealth)
{
	if (IsValid(Healthbar))
	{
		Healthbar->SetPercent(Health / MaxHealth);
	}

	if (IsValid(HealthText)) 
	{
		FText Format = FText::FromString(TEXT("{0} / {1}"));
		FText Text = FText::Format(Format, FText::AsNumber(Health), FText::AsNumber(MaxHealth));

		HealthText->SetText(Text);
	}
}
