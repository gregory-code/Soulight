// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SLineageCollection.h"
#include "Widgets/SLineageEntry.h"

#include "Components/ScrollBox.h"
#include "Components/VerticalBoxSlot.h"

#include "Engine/TextureRenderTarget2D.h"

#include "Framework/SGameInstance.h"

void USLineageCollection::NativeConstruct()
{
    Super::NativeConstruct();

    GameInstance = Cast<USGameInstance>(GetGameInstance());
}

UTexture2D* USLineageCollection::GetAbilityIcon(FString AbilityName) const
{
    if (GameInstance->AbilityIcons.Contains(AbilityName))
        return GameInstance->AbilityIcons[AbilityName];

    return nullptr;
}

void USLineageCollection::CreateLineageCollection(TArray<FLineageCharacterData> CurrentLineage)
{
    if (!IsValid(GameInstance->GetLineageEntryClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Lineage Entry Class Is Null"));

        return;
    }

	for (const FLineageCharacterData& Entry : CurrentLineage)
	{
        USLineageEntry* NewEntryWidget = CreateWidget<USLineageEntry>(GetWorld(), GameInstance->GetLineageEntryClass());
        InstancedEntries.Add(NewEntryWidget);

        if (IsValid(NewEntryWidget))
        {
            NewEntryWidget->InitLineageEntry
            (
                Entry.GetCharacterName().ToString(),
                Entry.GetPersonality().ToString(),
                GetAbilityIcon(Entry.GetSkillName()),
                GetAbilityIcon(Entry.GetSpellName()),
                GetAbilityIcon(Entry.GetPassiveName()),
                Entry.RenderTarget,
                GetAbilityIcon(Entry.GetInheritedAbility())
            );

            LineageScrollBox->AddChild(NewEntryWidget);

        }
	}

    GameInstance->CaptureChararcterPreview();
}

void USLineageCollection::UpdateLatestLineageEntry()
{
    if (GameInstance->GetCurrentLineage().Num() == 0 || InstancedEntries.Num() == 0) 
    {
        UE_LOG(LogTemp, Warning, TEXT("There Is No Current Lineage Yet!"));

        return;
    }

    if (!IsValid(InstancedEntries.Last())) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Last Instanced Entry Is Null!!"));

        return;
    }

    FLineageCharacterData LatestEntry = GameInstance->GetCurrentLineage().Last();

    InstancedEntries.Last()->InitLineageEntry
    (
        LatestEntry.GetCharacterName().ToString(),
        LatestEntry.GetPersonality().ToString(),
        GetAbilityIcon(LatestEntry.GetSkillName()),
        GetAbilityIcon(LatestEntry.GetSpellName()),
        GetAbilityIcon(LatestEntry.GetPassiveName()),
        LatestEntry.RenderTarget,
        GetAbilityIcon(LatestEntry.GetInheritedAbility())
    );
}

