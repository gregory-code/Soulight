// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SWritingTable.h"

#include "Abilities/SAbilityBase.h"

#include "Framework/SGameInstance.h"

#include "Player/SPlayer.h"

void ASWritingTable::BeginWrite(class ASPlayer* Player)
{

	if (!IsValid(Player)) return;

	USGameInstance* GameInstance = Cast<USGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance)) return;

	// TODO: Get UI for Inherited Ability Selection

	TArray<ASAbilityBase*> skillList;
	if(IsValid(Player->GetCurrentSkill()))
		skillList.Add(Player->GetCurrentSkill());
	if (IsValid(Player->GetCurrentSpell()))
		skillList.Add(Player->GetCurrentSpell());
	if (IsValid(Player->GetCurrentPassive()))
		skillList.Add(Player->GetCurrentPassive());

	if (skillList.IsEmpty()) return;

	int32 random = FMath::RandRange(0, skillList.Num() - 1);

	GameInstance->InheritAbility(skillList[random]);
}
