// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class USGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

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
