// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHealthbar.generated.h"

/**
 * 
 */
UCLASS()
class USHealthbar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateHealthbar(float Health, float MaxHealth);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Healthbar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

};
