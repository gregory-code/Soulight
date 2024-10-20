// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SLamppost.h"

#include "Framework/SFogCleaner.h"
#include "Player/SPlayer.h"

void ASLamppost::Interact(bool bActionable)
{
	Super::Interact(bActionable);

	if (!IsValid(FogCleanerClass)) return;

	FActorSpawnParameters spawnParam;
	FVector spawnPos = GetActorLocation();

	if(IsValid(Player))
		Player->OnInteract.RemoveDynamic(this, &ASInteractableObject::Interact);

	FogCleaner = GetWorld()->SpawnActor<ASFogCleaner>(FogCleanerClass, spawnPos, FRotator(0, 0, 0), spawnParam);
	if (IsValid(FogCleaner)) 
	{
		FogCleaner->SetColliderRadius(1000.0f);
	}
}
