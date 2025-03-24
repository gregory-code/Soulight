// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SLevelLoadInteract.h"

#include "Kismet/GameplayStatics.h"

void ASLevelLoadInteract::LoadLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}
