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
	
private:

	UPROPERTY(EditDefaultsOnly)
	FName IconMaterialParameterName{ "Icon" };

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemUpgrade;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;
};
