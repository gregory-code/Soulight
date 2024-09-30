// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/ForestPCGSetupActor.h"

AForestPCGSetupActor::AForestPCGSetupActor()
{
	PrimaryActorTick.bCanEverTick = true;

	/*

	// Create and attach PCG component
	PCGComponent = CreateDefaultSubobject<UPCGComponent>(TEXT("PCGComponent"));

	// Initialize with a random seed
	RandomStream.Initialize(FMath::Rand());
	*/
}

void AForestPCGSetupActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Generate a new seed

	// Set the seed to the PCG component
	//PCGComponent->SetSeed(RandomStream.GetCurrentSeed());
}


