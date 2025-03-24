// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SLineageAnimationPanel.h"

#include "Components/HorizontalBox.h"

#include "Framework/SGameInstance.h"

#include "Widgets/SLineageEntry.h"

void USLineageAnimationPanel::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UUserWidget*> ChildComps;
	for (UWidget* Child : EntryBoxContainer->GetAllChildren())
	{
		UUserWidget* UserWidget = Cast<UUserWidget>(Child);
		if (IsValid(UserWidget))
		{
			ChildComps.Add(UserWidget);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %d valid child components in EntryBoxContainer."), ChildComps.Num());

	for (UUserWidget* Child : ChildComps)
	{
		USLineageEntry* LineageEntry = Cast<USLineageEntry>(Child);
		if (IsValid(LineageEntry))
		{
			EntryPanels.Add(LineageEntry);
			UE_LOG(LogTemp, Log, TEXT("Added a valid LineageEntry to EntryPanels. Total Entries: %d"), EntryPanels.Num());
		}
	}

	USGameInstance* SGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (IsValid(SGameInstance))
	{
		GameInstance = SGameInstance;
		UE_LOG(LogTemp, Log, TEXT("GameInstance is valid and assigned."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is invalid!"));
	}

	ElapsedTime = -0.5f;
}

void USLineageAnimationPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ElapsedTime += InDeltaTime;

	if (ElapsedTime < 1.0f)
	{
		EntryBoxContainer->SetRenderTranslation(FVector2D(700.0f * -ElapsedTime, 0));
	}
	else if (ElapsedTime > 3) 
	{
		RemoveFromParent();
	}
}

UTexture2D* USLineageAnimationPanel::GetAbilityIcon(const FString& AbilityName) const
{
	if (GameInstance->AbilityIcons.Contains(AbilityName))
		return GameInstance->AbilityIcons[AbilityName];

	return nullptr;
}

void USLineageAnimationPanel::InitLineageEntries()
{
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is not valid in InitLineageEntries."));
		return;
	}

	int LineageCount = GameInstance->GetCurrentLineage().Num();
	UE_LOG(LogTemp, Log, TEXT("LineageCount: %d"), LineageCount);

	if (EntryPanels.Num() != 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("EntryPanels doesn't contain exactly 3 elements. Current Count: %d"), EntryPanels.Num());
		return;
	}

	// Ensure entries are placed from right to left
	int PanelIndex = EntryPanels.Num() - 1; // Start from the rightmost panel

	for (int i = LineageCount - 1; i >= 0; i--, PanelIndex--) // Go backwards through lineage
	{
		if (EntryPanels.IsValidIndex(PanelIndex))
		{
			USLineageEntry* EntryPanel = EntryPanels[PanelIndex]; // Assign newest to rightmost
			if (IsValid(EntryPanel))
			{
				const FLineageCharacterData& LineageData = GameInstance->GetCurrentLineage()[i];

				UE_LOG(LogTemp, Log, TEXT("Placing Lineage Entry %d (Character: %s) at UI Panel %d"),
					i, *LineageData.GetCharacterName().ToString(), PanelIndex);

				EntryPanel->InitLineageEntry
				(
					LineageData.GetCharacterName().ToString(),
					LineageData.GetPersonality().ToString(),
					GetAbilityIcon(LineageData.GetSkillName()),
					GetAbilityIcon(LineageData.GetSpellName()),
					GetAbilityIcon(LineageData.GetPassiveName()),
					LineageData.RenderTarget,
					GetAbilityIcon(LineageData.GetInheritedAbility())
				);

				EntryPanel->SetRenderOpacity(1.0f); // Ensure it is visible
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("EntryPanel at index %d is invalid!"), PanelIndex);
			}
		}
	}

	// Hide unused panels (if there are fewer than 3 entries)
	for (int i = PanelIndex; i >= 0; i--)
	{
		if (EntryPanels.IsValidIndex(i))
		{
			UE_LOG(LogTemp, Log, TEXT("Hiding EntryPanel at Index: %d"), i);
			EntryPanels[i]->SetRenderOpacity(0.0f);
		}
	}
}
