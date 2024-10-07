// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SItemUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USItemUI::SetItem(FString itemName, FString itemUpgrade, UTexture* itemImage, FColor itemColor)
{
	ItemName->SetText(FText::FromString(itemName));
	ItemUpgrade->SetText(FText::FromString(itemUpgrade));
	ItemUpgrade->SetColorAndOpacity(FSlateColor(itemColor));

	if (ItemImage == nullptr)
	{
		return;
	}

	ItemImage->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParameterName, itemImage);
}
