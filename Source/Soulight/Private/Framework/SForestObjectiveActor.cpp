// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SForestObjectiveActor.h"

#include "Kismet/GameplayStatics.h"

ASForestObjectiveActor::ASForestObjectiveActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASForestObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	ActiveLampCount = 0;
}

void ASForestObjectiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveLampCount >= TargetLampCount) 
	{
		UGameplayStatics::OpenLevel(this, CastleLevelName);
	}
}

