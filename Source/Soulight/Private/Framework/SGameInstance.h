// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

/**
 * 
 */
UCLASS()
class USGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	///////////////////////////////
	/*         Player           */
	/////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FEquippedItems EquippedItems;

	void ClearEquippedItems();

	///////////////////////////////
	/*         Lineage          */
	/////////////////////////////

	void InheritAbility(ASAbilityBase* Ability);

	UPROPERTY()
	ASAbilityBase* InheritedAbility;

	///////////////////////////////
	/*         Dungeon          */
	/////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	int32 CurrentProgress = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	int32 MaxProgress = 4;

	int32 PrevProgess = 0;

	void UpdateProgress();

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
};
