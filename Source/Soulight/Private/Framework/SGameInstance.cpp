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

	FDialogueData* Row = MyDataTable->FindRow<FDialogueData>(Personality, FString(""));

    if (!Row) return FString("Row Is Null!");

    FString Response = "!";
    FName InteractionPropertyName = FName(*InteractionName);

    UStruct* RowStruct = FDialogueData::StaticStruct();
    for (TFieldIterator<FProperty> PropIt(RowStruct); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;

        if (Property->GetFName() == InteractionPropertyName)
        {
            FStrProperty* StrProp = CastField<FStrProperty>(Property);
            if (StrProp)
            {
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
