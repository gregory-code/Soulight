// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SLevelLoadInteract.h"

#include "Kismet/GameplayStatics.h"

void ASLevelLoadInteract::Interact()
{
	Super::Interact();

	FTimerHandle LoadLevelHandle;

	GetWorld()->GetTimerManager().SetTimer(LoadLevelHandle, this, &ASLevelLoadInteract::LoadLevel, 1.0f, false, 0.1f);
}

void ASLevelLoadInteract::LoadLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}
