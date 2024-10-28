// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SAbility_Fireball.h"

void ASAbility_Fireball::ExecuteAbility()
{
	Super::ExecuteAbility();

	if (IsValid(FireballActorClass))
		GetWorld()->SpawnActor<AActor>(FireballActorClass);
}

void ASAbility_Fireball::EndAbility()
{
	Super::EndAbility();
}
