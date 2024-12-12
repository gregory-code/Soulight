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

	void SetSkillImage(UTexture* targetImage);
	void SetSpellImage(UTexture* targetImage);

private:
	UPROPERTY(EditDefaultsOnly)
		FName IconMaterialParameterName {
		"Icon"
	};

	UPROPERTY(meta = (BindWidget))
	class USMinimap* Minimap;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* SpellImage;

};
