// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SPlayerHUDUI.h"
#include "Widgets/SStatsUI.h"
#include "Widgets/SLineageCollection.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/TextureRenderTarget2D.h"

#include "Framework/SGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"

#include "PaperSprite.h"
#include "Player/SPlayer.h"

#include "InputCoreTypes.h"

void USPlayerHUDUI::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("Constructing SPlayerHUDUI"));

	if (!GetOwningPlayer()) return;

	APlayerController* PC = GetOwningPlayer();

	ASPlayer* SPlayer = Cast<ASPlayer>(PC->GetPawn());
	if (SPlayer)
	{
		Player = SPlayer;
	}

	// Unbind any previously bound inputs
	if (CachedEnhancedInput)
	{
		CachedEnhancedInput->ClearActionBindings();
	}

	// Store the reference to avoid duplicate bindings
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(GetOwningPlayer()->InputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Binding Input Contexts"));

		CachedEnhancedInput = EnhancedInput;

		EnhancedInput->BindAction(IA_UI_MenuLeft, ETriggerEvent::Started, this, &USPlayerHUDUI::MenuLeft);
		EnhancedInput->BindAction(IA_UI_MenuRight, ETriggerEvent::Started, this, &USPlayerHUDUI::MenuRight);
	}

	if (USGameInstance* SGameInstance = Cast<USGameInstance>(GetGameInstance()))
	{
		if (SGameInstance->GetCurrentLineage().Num() > 0)
		{
			UTextureRenderTarget2D* RenderTarget2D = SGameInstance->GetCurrentLineage().Last().RenderTarget;
			if (RenderTarget2D)
			{
				UMaterialInstanceDynamic* DynamicMaterial = CharacterRenderPreview->GetDynamicMaterial();
				if (DynamicMaterial)
				{
					DynamicMaterial->SetTextureParameterValue(TEXT("RenderTarget2D"), RenderTarget2D);
				}
			}
		}
	}
	
	ResumeBtn->OnClicked.AddDynamic(this, &USPlayerHUDUI::Resume);
	SettingsBtn->OnClicked.AddDynamic(this, &USPlayerHUDUI::Settings);
	QuitBtn->OnClicked.AddDynamic(this, &USPlayerHUDUI::QuitGame);

	MainTabsText = { EquipmentTab, LineageTab, SettingsTab };
	UpdateActiveTab();

	if (CurrentTabIndex == 2)
	{
		ResumeBtn->SetFocus();
	}

	UIWidgetSwitcher->SetActiveWidgetIndex(CurrentTabIndex);
}

void USPlayerHUDUI::NativeDestruct()
{
	Super::NativeDestruct();

	if (CachedEnhancedInput)
	{
		CachedEnhancedInput->ClearActionBindings();
		CachedEnhancedInput = nullptr;
	}

	ResumeBtn->OnClicked.RemoveDynamic(this, &USPlayerHUDUI::Resume);
	SettingsBtn->OnClicked.RemoveDynamic(this, &USPlayerHUDUI::Settings);
	QuitBtn->OnClicked.RemoveDynamic(this, &USPlayerHUDUI::QuitGame);
}


void USPlayerHUDUI::MenuLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Menuing Left"));

	MenuTab(-1);
}

void USPlayerHUDUI::MenuRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Menuing Right"));

	MenuTab(1);
}

void USPlayerHUDUI::MenuTab(const int32& Direction)
{
	CurrentTabIndex += Direction;

	if (CurrentTabIndex < 0) CurrentTabIndex = 0;
	if (CurrentTabIndex > 2) CurrentTabIndex = 2;

	UpdateActiveTab();

	if (CurrentTabIndex == 2)
	{
		ResumeBtn->SetFocus();
	}

	UIWidgetSwitcher->SetActiveWidgetIndex(CurrentTabIndex);
}

void USPlayerHUDUI::UpdateActiveTab()
{
	for (int32 i = 0; i < MainTabsText.Num(); i++)
	{
		if (MainTabsText[i])
		{
			if (i == CurrentTabIndex)
			{
				// Highlight Active Tab (e.g., White)
				MainTabsText[i]->SetColorAndOpacity(FSlateColor(FLinearColor::White));
			}
			else
			{
				// Dim Inactive Tabs (e.g., Gray)
				MainTabsText[i]->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
			}
		}
	}
}

void USPlayerHUDUI::UpdateEquipmentIcon(UPaperSprite* Icon, EEquipmentType EquipmentType)
{
	if (!Icon) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(Icon);

	switch (EquipmentType) 
	{
		case EEquipmentType::HEAD:
			HeadEquipment->SetBrush(Brush);
			break;
		case EEquipmentType::CHEST:
			ChestEquipment->SetBrush(Brush);
			break;
		case EEquipmentType::BOOTS:
			BootsEquipment->SetBrush(Brush);
			break;
		case EEquipmentType::WEAPON:
			SwordEquipment->SetBrush(Brush);
			break;
	}
}

void USPlayerHUDUI::SetStats(float Str, float Def, float Agi, float Soul)
{
	FText Text = FText::Format(FTextFormat::FromString("Strength: {0}"), FText::AsNumber(Str));
	CharacterStrength->SetText(Text);

	Text = FText::Format(FTextFormat::FromString("Defense: {0}"), FText::AsNumber(Def));
	CharacterDefense->SetText(Text);

	Text = FText::Format(FTextFormat::FromString("Agility: {0}"), FText::AsNumber(Agi));
	CharacterAgility->SetText(Text);

	Text = FText::Format(FTextFormat::FromString("Soul: {0}"), FText::AsNumber(Soul));
	CharacterSoul->SetText(Text);
}

void USPlayerHUDUI::CreateLineage()
{
	USGameInstance* GameInstance = Cast<USGameInstance>(GetGameInstance());

	LineageCollection->CreateLineageCollection(GameInstance->GetCurrentLineage());
}

void USPlayerHUDUI::Resume()
{
	if (!Player) return;

	Player->HUD();
}

void USPlayerHUDUI::Settings()
{

}

void USPlayerHUDUI::QuitGame()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	}
}

