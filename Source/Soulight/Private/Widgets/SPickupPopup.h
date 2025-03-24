// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPickupPopup.generated.h"

/**
 * 
 */
UCLASS()
class USPickupPopup : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitIcon(UTexture* PickupIconTexture, FString PickupItemName);
	void InitIconSprite(class UPaperSprite* PickupIconTexture, FString PickupItemName);

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

};
