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
		for (int x = 0; x < FogColumns; x++)
		{
			FVector spawnAdjustments = FVector((i * FogSpacing), (x * FogSpacing), 0);
			FVector spawnPos = FogSpawnPosition + spawnAdjustments;

			ASFog* newFog = GetWorld()->SpawnActor<ASFog>(mFogClass, spawnPos, FRotator(0, 0, 0), spawnParam);
		}
	}

}
