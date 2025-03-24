// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SPickupPopup.h"

#include "PaperSprite.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void USPickupPopup::InitIcon(UTexture* PickupIconTexture, FString PickupItemName)
{
	if (IsValid(PickupIconTexture)) 
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(PickupIconTexture);

		Icon->SetBrush(Brush);
	}

	FText Text = FText::FromString(PickupItemName);
	ItemName->SetText(Text);
}

void USPickupPopup::InitIconSprite(UPaperSprite* PickupIconTexture, FString PickupItemName)
{
	if (IsValid(PickupIconTexture))
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(PickupIconTexture);

		Icon->SetBrush(Brush);
	}

	FText Text = FText::FromString(PickupItemName);
	ItemName->SetText(Text);
}
