 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SAbilityDataBase.generated.h"


UENUM(BlueprintType)
enum class EType : uint8
{
	Passive  UMETA(DisplayName = "Passive"),
	Skill    UMETA(DisplayName = "Skill"),
	Spell    UMETA(DisplayName = "Spell")
};

UENUM(BlueprintType)
enum class EUpgrade : uint8
{
	New			 UMETA(DisplayName = "New"),
	Upgrade		 UMETA(DisplayName = "Upgrade"),
	Replace		  UMETA(DisplayName = "Replace")
};

class UAnimMontage;
/**
 * 
 */
UCLASS()
class USAbilityDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	///////////////////////////////
	/*        Gameplay          */
	/////////////////////////////
	
	UFUNCTION()
	bool LevelUp();

	UFUNCTION()
	int GetLevel();

	UFUNCTION()
	EType GetType();

	UFUNCTION()
	float GetBaseDamage();

	UFUNCTION()
	float GetAnimSpeedMultiplier();
	
	UFUNCTION()
	UAnimMontage* GetAnimationMontage() { return AbilityAnimation; }

	///////////////////////////////
	/*           UI             */
	/////////////////////////////
	UFUNCTION()
	FString GetAbilityName();

	UFUNCTION()
	FString GetAbilityDescription();

	UFUNCTION()
	UTexture* GetAbilityIcon();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* AbilityAnimation;
	
private:
	int level = 0;

	UPROPERTY(EditDefaultsOnly)
	EType AbilityType;

	UPROPERTY(EditDefaultsOnly, Category = "Style")
	FString AbilityName;

	UPROPERTY(EditDefaultsOnly, Category = "Style")
	UTexture* AbilityIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Style")
	FString AbilityDescription;

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	TArray<float> BaseDamage = { 10, 25, 50 };

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	TArray<float> AnimSpeedMultiplier = { 1,2, 3 };
};
