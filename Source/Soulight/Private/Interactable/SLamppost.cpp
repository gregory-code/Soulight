// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SLamppost.h"

#include "Components/PointLightComponent.h"

#include "Framework/SFogCleaner.h"
#include "Framework/SoulightGameMode.h"
#include "Framework/SForestObjectiveActor.h"

#include "Player/SPlayer.h"

#include "Kismet/GameplayStatics.h"

ASLamppost::ASLamppost()
{
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Lamp Light"));
	PointLight->SetupAttachment(RootComponent);
}

void ASLamppost::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);

	SoulightGameMode = Cast<ASoulightGameMode>(GameMode);

	if (IsValid(PointLight))
	{
		PointLight->SetVisibility(false);
	}
}

void ASLamppost::Interact()
{
	Super::Interact();

	if (!IsValid(FogCleanerClass)) return;

	FActorSpawnParameters spawnParam;
	FVector spawnPos = GetActorLocation();
	
	if (IsValid(SoulightGameMode)) 
	{
		SoulightGameMode->GetForestObjectiveTracker()->ActiveLampCount++;
		UE_LOG(LogTemp, Warning, TEXT("Incrementing Lamp Count"));
	}

	FogCleaner = GetWorld()->SpawnActor<ASFogCleaner>(FogCleanerClass, spawnPos, FRotator(0, 0, 0), spawnParam);
	if (IsValid(FogCleaner)) 
	{
		FogCleaner->SetColliderRadius(1000.0f);
	}

	if (IsValid(PointLight))
	{
		PointLight->SetVisibility(true);
	}
}
