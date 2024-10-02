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
	void SetItem(FString itemName, FString itemDescription, UTexture* itemImage);
	
private:

	UPROPERTY(EditDefaultsOnly)
	FName IconMaterialParameterName{ "Icon" };

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDescription;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;
};
