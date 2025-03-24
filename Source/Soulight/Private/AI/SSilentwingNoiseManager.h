// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Object.h"
#include "SSilentwingNoiseManager.generated.h"

class ASEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNoiseReported, float, CurrentNoiseLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoiseReset);

UCLASS()
class USSilentwingNoiseManager : public UObject
{
	GENERATED_BODY()
public:	
	USSilentwingNoiseManager();

	static USSilentwingNoiseManager& Get(const UObject* ContextObject);

	FOnNoiseReported OnNoiseReported;
	FOnNoiseReset OnNoiseReset;

public:
	UFUNCTION()
	void GenerateNoise(const float& Noise, const FVector& NoiseLocation);
	bool IsFirstSpawn() const { return bFirstSpawn; }

protected:
	void SpawnSilentwing(const FVector& NoiseLocation);

	UPROPERTY(EditDefaultsOnly, Category = "Silentwing")
	float NoiseCap = 100.f;
	float TotalNoise = 0;

private:
	UFUNCTION()
	void StartLoseNoiseHeard();
	bool bFirstSpawn = true;

	UFUNCTION()
	void StartSilentWingSpawnDelay();

	UPROPERTY(EditDefaultsOnly, Category = "Silentwing")
	TSubclassOf<AActor> SilentwingEnemyClass;

	FTimerHandle NextSilentWingDelayHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Silentwing")
	float NextSilentWingSpawnDelay = 5.f;
	bool bSilentwingDelayActive = false;

	FTimerHandle NoiseGeneratedHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Silentwing")
	float NoiseLossTimerDelay = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Silentwing")
	float NoiseLossBleedRate = 1.f;


};
