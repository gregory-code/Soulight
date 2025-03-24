// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SEquipment.h"

#include "Player/SPlayer.h"

void ASEquipment::EquipItem(class ASPlayer* Player)
{
	if (Player == nullptr || Equipment == nullptr) return;

	Player->EquipItem(Equipment);

	this->Destroy();
}
