// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SItemUI.generated.h"

/**
 * 
 */
UCLASS()
class USItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItem(FString itemName, FString itemUpgrade, UTexture* itemImage, FColor itemColor);
	
	void TickInRange(bool bInRange, float DeltaTime);

	void Start();

private:

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(EditDefaultsOnly)
	FName IconMaterialParameterName{ "Icon" };

	UPROPERTY(EditDefaultsOnly)
	FName OutlineParameterName{ "Outline Box Scale" };

	UPROPERTY(EditDefaultsOnly)
	FName BackgroundParameterName{ "Box Scale" };

	UPROPERTY(EditDefaultsOnly)
	FName OpacityParameterName{ "Opacity" };

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemUpgrade;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY()
	class UCanvasPanelSlot* ItemNameSlot;

	UPROPERTY()
	class UCanvasPanelSlot* ItemUpgradeSlot;

	UPROPERTY()
	float OriginalOutlineScale;

	UPROPERTY()
	float OriginalBackgroundScale;

	UPROPERTY()
	float OriginalItemNameY;

	UPROPERTY()
	float OriginalItemUpgradeY;
};
