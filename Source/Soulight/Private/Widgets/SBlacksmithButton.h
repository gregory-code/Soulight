// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBlacksmithButton.generated.h"

/**
 * 
 */
UCLASS()
class USBlacksmithButton : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClicked();

	UPROPERTY(EditAnywhere, Category = "Soul")
	class USEquipmentData* EquipmentObject;

	UPROPERTY(EditAnywhere, Category = "Soul")
	FText NameOfItem;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectBtn;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	class UImage* TargetImage;

};
