// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SItemUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USItemUI::SetItem(FString itemName, FString itemDescription, UTexture* itemImage)
{
	ItemName->SetText(FText::FromString(itemName));
	ItemDescription->SetText(FText::FromString(itemDescription));

	if (ItemImage == nullptr)
	{
		return;
	}

	ItemImage->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParameterName, itemImage);
}
