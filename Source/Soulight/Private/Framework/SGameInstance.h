// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Framework/SCharacterBase.h"

#include "SGameInstance.generated.h"

class ASAbilityBase;
class USEquipmentData;

USTRUCT(BlueprintType)
struct FEquippedItems 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	ASAbilityBase* EquippedSkill;

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	ASAbilityBase* EquippedSpell;

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	ASAbilityBase* EquippedPassive;

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	USEquipmentData* EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	USEquipmentData* EquippedChest;

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	USEquipmentData* EquippedHead;

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	USEquipmentData* EquippedBoot;

	FEquippedItems()
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating FEquippedItems Struct!"));
	}

};


/*
*	This is a lot of variables only because Unreal demands you use a custom
*	Struct in c++ in order to read the row data. Otherwise we could've just
*	used a structure in the editor.
*/
USTRUCT(BlueprintType)
struct FDialogueData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Arrival_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString TalkToBlacksmith_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString TalkToFisher_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString TalkToMerchant_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString EnterForest_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString GrabbedByMistHand_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString LightsLampost_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString RevealsProwlerWithLight_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString HearsWendigo_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString SeesArborelHive_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString EnterCastle_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString SeesDreadstormFireball_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString OpensChest_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString TriggersMimic_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString SendsOffLetter_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString FindsSoul_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString KillsEnemy_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString EquipsMaxTierItem_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString ObtainsMaxTierAbility_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString SeesDreadstorm_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Dies_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString WinsGame_Default;
};

USTRUCT(BlueprintType)
struct FLineageEntry
{
	GENERATED_BODY()

public:
	FLineageEntry()
		: Personality(NAME_None), InheritedAbility(nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Default Lineage Entry Created"));
	}

	FLineageEntry(TArray<FName> Personalities, TArray<TSubclassOf<ASAbilityBase>> Abilities)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating Lineage Entry"));

		if (Personalities.Num() > 0)
		{
			int rand = FMath::RandRange(0, Personalities.Num() - 1);
			Personality = Personalities[rand];
		}

		if (Abilities.Num() > 0)
		{
			int rand = FMath::RandRange(0, Abilities.Num() - 1);
			InheritedAbility = Abilities[rand];
		}
	}

	FName GetPersonality() const { return Personality; }

	TSubclassOf<ASAbilityBase> GetAbility() const { return InheritedAbility; }

private:
	FName Personality;

	TSubclassOf<ASAbilityBase> InheritedAbility;

};

/**
 * 
 */
UCLASS()
class USGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
#pragma region Player Variables/Functions

	///////////////////////////////
	/*        Dialogue          */
	/////////////////////////////
	UFUNCTION(BlueprintCallable)
	FString GetResponse(const FName& Personality, const FString& InteractionName) const;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	class UDataTable* MyDataTable;

#pragma endregion

#pragma region Player Variables/Functions

	///////////////////////////////
	/*         Player           */
	/////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	ESoulStatType CurrentSoulStatType;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FEquippedItems EquippedItems;

	void ClearEquippedItems();

#pragma endregion

#pragma region Dungeon Variables/Functions

	///////////////////////////////
	/*         Dungeon          */
	/////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	int32 CurrentProgress = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	int32 MaxProgress = 4;

	int32 PrevProgess = 0;

	void UpdateProgress();

#pragma endregion

#pragma region Fog Variables/Functions

	///////////////////////////////
	/*            Fog           */
	/////////////////////////////

	UFUNCTION()
	void CreateFog();

	UPROPERTY(EditDefaultsOnly, Category = "Fog")

	TSubclassOf<class ASFog> mFogClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	TSubclassOf<class ASFog> mCursedFogClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	FVector FogSpawnPosition;

	FTimerHandle FogTimerHandle;
	void ProcessSpawningFog(int rowPos, int columns, TSubclassOf<class ASFog> classToSpawn, const FActorSpawnParameters spawnParam);

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	int CursedFogAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	int FogRows;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	int FogColumns;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	int FogSpacing;

#pragma endregion 

#pragma region Lineage Variables/Functions

public:
	UFUNCTION()
	void StartLineage();

	UFUNCTION()
	void CreateNewLineage();

	UFUNCTION()
	FName GetCurrentPersonality();

	void InheritAbility(ASAbilityBase* Ability);

	UPROPERTY()
	ASAbilityBase* InheritedAbility;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Lineage")
	TArray<FName> Personalities;
	UPROPERTY(EditDefaultsOnly, Category = "Lineage")
	TArray<TSubclassOf<ASAbilityBase>> Abilities;

	FLineageEntry CurrentLineageEntry;
	FLineageEntry NextLineageEntry;

	TArray<FLineageEntry> PreviousLineageEntries;

#pragma endregion


};
