// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SSilentwingNoiseManager.h"
#include "Framework/SGameInstance.h"
#include "Kismet/GameplayStatics.h"

USSilentwingNoiseManager::USSilentwingNoiseManager()
{

}

void USSilentwingNoiseManager::GenerateNoise(const float& Noise, const FVector& NoiseLocation)
{
	if (bSilentwingDelayActive)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Silentwing Delay Is Currently Active"));
		return;
	}

	TotalNoise += Noise;
	//UE_LOG(LogTemp, Warning, TEXT("Noise Level: %f"), TotalNoise);

	OnNoiseReported.Broadcast(TotalNoise);

	if (TotalNoise >= NoiseCap)
	{
		SpawnSilentwing(NoiseLocation);

		bSilentwingDelayActive = true;

		GetWorld()->GetTimerManager().SetTimer(NextSilentWingDelayHandle, this, &USSilentwingNoiseManager::StartSilentWingSpawnDelay, 1 / 24.f, false, NextSilentWingSpawnDelay);

		TotalNoise = 0;
	}

	if (GetWorld()->GetTimerManager().IsTimerPending(NoiseGeneratedHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(NoiseGeneratedHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(NoiseGeneratedHandle, this, &USSilentwingNoiseManager::StartLoseNoiseHeard, 1/24.f, true, NoiseLossTimerDelay);
}

void USSilentwingNoiseManager::StartLoseNoiseHeard()
{
	TotalNoise -= NoiseLossBleedRate * GetWorld()->GetDeltaSeconds();
	//UE_LOG(LogTemp, Warning, TEXT("Noise Level: %f"), TotalNoise);

	if (TotalNoise <= 0)
	{
		TotalNoise = 0;

		OnNoiseReset.Broadcast();

		GetWorld()->GetTimerManager().ClearTimer(NoiseGeneratedHandle);
	}
}

void USSilentwingNoiseManager::StartSilentWingSpawnDelay()
{
	bSilentwingDelayActive = false;
	GetWorld()->GetTimerManager().ClearTimer(NextSilentWingDelayHandle);
}

void USSilentwingNoiseManager::SpawnSilentwing(const FVector& NoiseLocation)
{
	if (!IsValid(SilentwingEnemyClass)) return;

	bFirstSpawn = false;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnPosition = FVector(NoiseLocation.X - 200, NoiseLocation.Y - 200, NoiseLocation.Z + 500.f);
	//FVector SpawnPosition = FVector(NoiseLocation.X, NoiseLocation.Y, NoiseLocation.Z);

	GetWorld()->SpawnActor<AActor>(SilentwingEnemyClass, SpawnPosition, FRotator::ZeroRotator, SpawnParams);
}

USSilentwingNoiseManager& USSilentwingNoiseManager::Get(const UObject* ContextObject)
{
	USGameInstance* GameInstance = Cast<USGameInstance>(UGameplayStatics::GetGameInstance(ContextObject));
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Cannot get ASSilentwingNoiseManage singleton from the Game Instance, need to be of type SGameInstance"));
		return *NewObject<USSilentwingNoiseManager>(); // never calls this
	}	
	
	USSilentwingNoiseManager* NoiseManager = GameInstance->GetNoiseManager();
	if(!NoiseManager)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Cannot get ASSilentwingNoiseManage singleton from the  SGame Instance, maybe the SilentwingNoiseManagerClass is not specified on the game instance"));
		return *NewObject<USSilentwingNoiseManager>(); // never calls this
	}

	return *NoiseManager;
}
