// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SChest.h"

void ASChest::Interact(bool bActionable)
{
	Super::Interact(bActionable);

	if (ItemPool.Num() == 0) return;

	int32 RandomNumber = FMath::RandRange(0, ItemPool.Num() - 1);
	if (!IsValid(ItemPool[RandomNumber])) return;

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 10.0f;

	ASInteractableObject* InstancedObject = GetWorld()->SpawnActor<ASInteractableObject>(ItemPool[RandomNumber], SpawnLocation, GetActorRotation());

	Destroy();
}
