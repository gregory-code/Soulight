// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STextPrompt.generated.h"

/**
 * 
 */
UCLASS()
class USTextPrompt : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetText(const FText& TextToSet);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextPrompt;

};
