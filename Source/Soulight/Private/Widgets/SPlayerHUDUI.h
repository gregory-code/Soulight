// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPlayerHUDUI.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class USPlayerHUDUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	class USStatsUI* StatsUI;

	UFUNCTION()
	void SetStats(float Str, float Def, float Agi, float Soul);

	void CreateLineage();

	class USLineageCollection* GetLineageCollection() const { return LineageCollection; }

private:
	class ASPlayer* Player;

	class UEnhancedInputComponent* CachedEnhancedInput;

	UPROPERTY(EditDefaultsOnly, Category = "UI Input")
	class UInputAction* IA_UI_MenuLeft;

	UPROPERTY(EditDefaultsOnly, Category = "UI Input")
	class UInputAction* IA_UI_MenuRight;

	UPROPERTY(EditDefaultsOnly, Category = "UI Input")
	class UInputAction* IA_UI_Escape;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EquipmentTab;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LineageTab;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SettingsTab;

	TArray<UTextBlock*> MainTabsText;

	UFUNCTION()
	void MenuLeft();
	UFUNCTION()
	void MenuRight();

	void MenuTab(const int32& Direction);

	void UpdateActiveTab();

	int32 CurrentTabIndex = 0;

private:
	UPROPERTY(meta = (BindWidget))
	class USLineageCollection* LineageCollection;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterCurrentHP;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterMaxHP;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterStrength;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterDefense;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterAgility;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterSoul;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* UIWidgetSwitcher;

	// Equipment UI
public:
	UFUNCTION()
	void UpdateEquipmentIcon(class UPaperSprite* Icon, EEquipmentType EquipmentType);

private:
	// Equipment Icons
	UPROPERTY(meta = (BindWidget))
	UImage* CharacterRenderPreview;

	UPROPERTY(meta = (BindWidget))
	UImage* HeadEquipment;

	UPROPERTY(meta = (BindWidget))
	UImage* ChestEquipment;

	UPROPERTY(meta = (BindWidget))
	UImage* BootsEquipment;

	UPROPERTY(meta = (BindWidget))
	UImage* SwordEquipment;

	// Settings UI
public:
	UFUNCTION()
	void Resume();

	UFUNCTION()
	void Settings();

	UFUNCTION()
	void QuitGame();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ResumeBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitBtn;
};
