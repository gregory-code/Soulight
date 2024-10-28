// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SStatData.generated.h"

/**
 * 
 */
UCLASS()
class USStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION()
	float GetMaxHealthStat() const { return MaxHealth; }
	UFUNCTION()
	float GetStrengthStat() const { return Strength; }
	UFUNCTION()
	float GetDefenseStat() const { return Defense; }
	UFUNCTION()
	float GetAgilityStat() const { return Agility; }
	UFUNCTION()
	float GetSoulStat() const { return Soul; }

protected:
	///////////////////////////////
	/*          Stats           */
	/////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Strength = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Defense = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Agility = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Soul = 10.0f;
};
