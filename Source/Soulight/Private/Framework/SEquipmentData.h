// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SEquipmentData.generated.h"

UENUM(BlueprintType)
enum class EEquipmentType : uint8 
{
	HEAD	UMETA(DisplayName = "Head"),
	CHEST	UMETA(DisplayName = "Chest"),
	BOOTS	UMETA(DisplayName = "Boots"),
	WEAPON	UMETA(DisplayName = "Weapon")
};

UENUM(BlueprintType)
enum class EEquipmentRarity : uint8
{
	COMMON		UMETA(DisplayName = "Common"),
	UNCOMMON	UMETA(DisplayName = "Uncommon"),
	RARE		UMETA(DisplayName = "Rare")
};

/**
 * 
 */
UCLASS()
class USEquipmentData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	EEquipmentType EquipmentType;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	EEquipmentRarity EquipmentRarity;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	class UStaticMesh* EquipmentMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	class USStatData* EquipmentStats;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	FString EquipmentName;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	class UPaperSprite* EquipmentIcon;
};
