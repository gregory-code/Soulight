// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SAbilityBase_WarriorSpirit.h"
#include "Abilities/SAbilityBase.h"

#include "Framework/SCharacterBase.h"

void ASAbilityBase_WarriorSpirit::RegisterAbility(ACharacter* OwningCharacter)
{
	Super::RegisterAbility(OwningCharacter);

	if (!IsValid(OwnerCharacter)) return;

	Player = Cast<ASCharacterBase>(OwnerCharacter);
	if (IsValid(Player))
	{
		Player->OnDamageDealt.AddDynamic(this, &ASAbilityBase_WarriorSpirit::DamageDealt);
	}
}

void ASAbilityBase_WarriorSpirit::DamageDealt(ASCharacterBase* Target)
{
	if (!IsValid(Target)) return;

	UE_LOG(LogTemp, Warning, TEXT("Warrior Spirit Proc"))

	// TODO : Scale Duration with Soul
	Target->ApplyStun(1.0f, true);
}
