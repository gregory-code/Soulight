// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"

#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Framework/SFog.h"

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
