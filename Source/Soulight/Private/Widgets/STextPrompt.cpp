// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/STextPrompt.h"

#include "Components/TextBlock.h"

void USTextPrompt::SetText(const FText& TextToSet)
{
	if (!IsValid(TextPrompt)) return;

	TextPrompt->SetText(TextToSet);
}
