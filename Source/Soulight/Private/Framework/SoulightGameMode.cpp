// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SoulightGameMode.h"

#include "Abilities/SAbilityBase.h"

#include "Engine/World.h"

#include "Framework/SGameInstance.h"
#include "Framework/SForestObjectiveActor.h"

#include "GameFramework/PlayerInput.h"
#include "GameFramework/Actor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void ASoulightGameMode::BeginPlay()
{
	Super::BeginPlay();

	ForestObjectiveTracker = GetWorld()->SpawnActor<ASForestObjectiveActor>(ForestObjectiveTrackerClass);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (GameInstance)
	{
		USGameInstance* SoulightGameInstance = Cast<USGameInstance>(GameInstance);
		if (SoulightGameInstance)
		{
			SoulightGameInstance->CreateFog();
		}
	}
}
