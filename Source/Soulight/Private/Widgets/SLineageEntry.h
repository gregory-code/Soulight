// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLineageEntry.generated.h"

class UMaterialInterface;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class USLineageEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitLineageEntry(const FString& CharacterName, const FString& Personality,
		UTexture2D* SkillIcon, UTexture2D* SpellIcon, UTexture2D* PassiveIcon, class UTextureRenderTarget2D* CharacterRenderTarget,
		UTexture2D* InheritedAbility
	);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterPersonalityText;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterPreviewImage;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterSkillIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterSpellIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterPassiveIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterInheritedAbilityIcon;

};
