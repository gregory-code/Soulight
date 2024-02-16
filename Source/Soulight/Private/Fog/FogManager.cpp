// Fill out your copyright notice in the Description page of Project Settings.


#include "Fog/FogManager.h"
#include "Fog.h"
#include "Engine/World.h"

// Sets default values
AFogManager::AFogManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFogManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnClouds();
}

void AFogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFogManager::SpawnClouds()
{
	if (FogClass == nullptr)
		return;

	for (AFog* FogActor : FogArray)
	{
		if (FogActor)
		{
			FogActor->Destroy();
		}
	}
	FogArray.Empty();

	for (int x = 0; x < GridSizeX; x++)
	{
		for (int y = 0; y < GridSizeY; y++)
		{
			float RandomRotation = FMath::RandRange(0, 360);


			FVector SpawnLocation = FVector(x * CellSize, y * CellSize, 0.0f);
			SpawnLocation += GetActorLocation();
			FRotator SpawnRotation = FRotator(0.0f, RandomRotation, 0.0f);
			AFog* NewFog = GetWorld()->SpawnActor<AFog>(FogClass->GetClass(), SpawnLocation, SpawnRotation);
			FogArray.Add(NewFog);
		}
	}

}



