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
/**
 * 
 */
UCLASS()
class USAbilityDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool LevelUp();

	UFUNCTION()
	FString GetAbilityName();

	UFUNCTION()
	float GetBaseDamage();

	UFUNCTION()
	float GetAnimSpeedMultiplier();
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	class UAnimMontage* AbilityAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	TArray<float> BaseDamage = { 0, 0, 0 };

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	TArray<float> AnimSpeedMultiplier = { 0,0,0 };
};
