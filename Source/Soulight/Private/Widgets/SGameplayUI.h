// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameplayUI.generated.h"

/**
 * 
 */
UCLASS()
class USGameplayUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Begin Play or more compareable: Constructor
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class USMinimap* Minimap;
};
