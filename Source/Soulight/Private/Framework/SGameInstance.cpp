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

	FActorSpawnParameters SpawnParams;

	ASFog* newFog = GetWorld()->SpawnActor<ASFog>(mFogClass, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
}
