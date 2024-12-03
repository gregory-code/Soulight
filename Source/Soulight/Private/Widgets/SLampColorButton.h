// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SCharacterBase.h"
#include "SLampColorButton.generated.h"

/**
 * 
 */
UCLASS()
class USLampColorButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClicked();

	UPROPERTY(EditAnywhere, Category = "Soul")
	ESoulStatType SoulStatType;

	UPROPERTY(EditAnywhere, Category = "Soul")
	FText PassiveText;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectBtn;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PassiveDescription;
	UPROPERTY(meta = (BindWidget))
	class UImage* TargetImage;

};
