// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SItemUI.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void USItemUI::Start()
{
	OriginalOutlineScale = ItemImage->GetDynamicMaterial()->K2_GetScalarParameterValue(OutlineParameterName);
	OriginalBackgroundScale = ItemImage->GetDynamicMaterial()->K2_GetScalarParameterValue(BackgroundParameterName);

	ItemImage->GetDynamicMaterial()->SetScalarParameterValue(OpacityParameterName, 0);
	ItemImage->GetDynamicMaterial()->SetScalarParameterValue(OutlineParameterName, 0);
	ItemImage->GetDynamicMaterial()->SetScalarParameterValue(BackgroundParameterName, 0);

	ItemNameSlot = Cast<UCanvasPanelSlot>(ItemName->Slot);
	ItemUpgradeSlot = Cast<UCanvasPanelSlot>(ItemUpgrade->Slot);

	OriginalItemNameY = ItemNameSlot->GetPosition().Y;
	OriginalItemUpgradeY = ItemUpgradeSlot->GetPosition().Y;
}

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

void USItemUI::TickInRange(bool bInRange, float DeltaTime)
{
	if (ItemNameSlot == nullptr)
		return;

	float opacity = ItemImage->GetDynamicMaterial()->K2_GetScalarParameterValue(OpacityParameterName);
	float outlineScale = ItemImage->GetDynamicMaterial()->K2_GetScalarParameterValue(OutlineParameterName);
	float backgroundScale = ItemImage->GetDynamicMaterial()->K2_GetScalarParameterValue(BackgroundParameterName);
	float itemNameY = ItemNameSlot->GetPosition().Y;
	float itemUpgradeY = ItemUpgradeSlot->GetPosition().Y;

	opacity = FMath::Lerp(opacity, (bInRange) ? 1.0 : 0, 6.0f * DeltaTime);
	outlineScale = FMath::Lerp(outlineScale, (bInRange) ? OriginalOutlineScale : 0, 6.0f * DeltaTime);
	backgroundScale = FMath::Lerp(backgroundScale, (bInRange) ? OriginalBackgroundScale : 0, 6.0f * DeltaTime);
	itemNameY = FMathf::Lerp(itemNameY, (bInRange) ? OriginalItemNameY : 0, 6.0f * DeltaTime);
	itemUpgradeY = FMathf::Lerp(itemUpgradeY, (bInRange) ? OriginalItemUpgradeY : 0, 6.0f * DeltaTime);

	FLinearColor itemNameColor = ItemName->ColorAndOpacity.GetSpecifiedColor();
	itemNameColor.A = opacity;
	ItemName->SetColorAndOpacity(FSlateColor(itemNameColor));

	FLinearColor itemUpgradeColor = ItemUpgrade->ColorAndOpacity.GetSpecifiedColor();
	itemUpgradeColor.A = opacity;
	ItemUpgrade->SetColorAndOpacity(FSlateColor(itemUpgradeColor));

	ItemImage->GetDynamicMaterial()->SetScalarParameterValue(OpacityParameterName, opacity);
	ItemImage->GetDynamicMaterial()->SetScalarParameterValue(OutlineParameterName, outlineScale);
	ItemImage->GetDynamicMaterial()->SetScalarParameterValue(BackgroundParameterName, backgroundScale);

	ItemNameSlot->SetPosition(FVector2D(0, itemNameY));
	ItemUpgradeSlot->SetPosition(FVector2D(0, itemUpgradeY));
}
