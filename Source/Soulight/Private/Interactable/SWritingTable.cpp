// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SWritingTable.h"

#include "Abilities/SAbilityBase.h"

#include "Framework/SGameInstance.h"

#include "Player/SPlayer.h"

void ASWritingTable::Interact()
{
	Super::Interact();

	if (!IsValid(Player)) return;

	USGameInstance* GameInstance = Cast<USGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance)) return;

	// TODO: Get UI for Inherited Ability Selection

	// Change this, just directly passing in the skill
	if(IsValid(Player->GetCurrentSkill()))
		GameInstance->InheritAbility(Player->GetCurrentSkill());
}
