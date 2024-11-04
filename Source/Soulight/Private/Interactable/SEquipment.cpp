// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SEquipment.h"

#include "Player/SPlayer.h"
#include "Framework/SEquipmentData.h"

void ASEquipment::Interact()
{
	Super::Interact();

	if (!IsValid(Player)) return;

	if (!IsValid(Equipment)) return;

	Player->EquipItem(Equipment);

	Destroy();
}
