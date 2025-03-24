// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SAbilityBase.h"
#include "SAbilityBase_WarriorSpirit.generated.h"

class ASCharacterBase;
/**
 * 
 */
UCLASS()
class ASAbilityBase_WarriorSpirit : public ASAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void RegisterAbility(ACharacter* OwningCharacter) override;

protected:
	UFUNCTION()
	void DamageDealt(ASCharacterBase* Target);

private:
	class ASCharacterBase* Player;

};
