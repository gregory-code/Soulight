// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SFog.h"

// Sets default values
ASFog::ASFog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASFog::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

