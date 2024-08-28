// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SoulightGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "Framework/SGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void ASoulightGameMode::BeginPlay()
{
	Super::BeginPlay();

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
