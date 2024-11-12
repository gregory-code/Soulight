// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SGameInstance.h"

#include "Engine/World.h"
#include "Engine/DataTable.h"

#include "Framework/SFog.h"

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerInput.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

FString USGameInstance::GetResponse(const FName& Personality, const FString& InteractionName) const
{
    if (!MyDataTable) return FString("Data Table Is Null!");

    // Find the row for the specified tone
	FDialogueData* Row = MyDataTable->FindRow<FDialogueData>(Personality, FString(""));

    if (!Row) return FString("Row Is Null!");

    // Use reflection to search for the property by name
    FString Response = "!";
    FName NPCPropertyName = FName(*InteractionName);

    // Get the class of the struct
    UStruct* RowStruct = FDialogueData::StaticStruct();

    // Loop over all properties in the struct
    for (TFieldIterator<FProperty> PropIt(RowStruct); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;

        // Check if the property name matches our target column (e.g., "NPC1Response", "NPC2Response")
        if (Property->GetFName() == NPCPropertyName)
        {
            // Cast and retrieve the value from the row struct
            FStrProperty* StrProp = CastField<FStrProperty>(Property);
            if (StrProp)
            {
                // Access the property value and set it to Response
                Response = StrProp->GetPropertyValue_InContainer(Row);
                break;
            }
        }
    }

	return Response;
}

void USGameInstance::ClearEquippedItems()
{
	EquippedItems.EquippedSkill = nullptr;
	EquippedItems.EquippedSpell = nullptr;
	EquippedItems.EquippedPassive = nullptr;
	EquippedItems.EquippedWeapon = nullptr;
	EquippedItems.EquippedChest = nullptr;
	EquippedItems.EquippedHead = nullptr;
	EquippedItems.EquippedBoot = nullptr;
}

void USGameInstance::InheritAbility(ASAbilityBase* Ability)
{
	InheritedAbility = Ability;
}

void USGameInstance::UpdateProgress()
{
	PrevProgess = CurrentProgress;
}

void USGameInstance::CreateFog()
{
	if (mFogClass == nullptr)
		return;

	FActorSpawnParameters spawnParam;
	for (int i = 0; i < FogRows; i++)
	{
		FogTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &USGameInstance::ProcessSpawningFog, (i * FogSpacing), 0, (i < CursedFogAmount || i > FogRows - CursedFogAmount) ? mCursedFogClass : mFogClass, spawnParam));
	}
}

void USGameInstance::ProcessSpawningFog(int rowPos, int columns, TSubclassOf<class ASFog> classToSpawn, const FActorSpawnParameters spawnParam)
{
	if (columns >= FogColumns)
	{
		return;
	}

	FVector spawnAdjustments = FVector(rowPos, (columns * FogSpacing), 200);
	FVector spawnPos = FogSpawnPosition + spawnAdjustments;

	ASFog* newFog = GetWorld()->SpawnActor<ASFog>((columns < CursedFogAmount || columns > FogColumns - CursedFogAmount) ? mCursedFogClass : classToSpawn, spawnPos, FRotator(0, 0, 0), spawnParam);

	columns++;
	FogTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &USGameInstance::ProcessSpawningFog, rowPos, columns, classToSpawn, spawnParam));
}
