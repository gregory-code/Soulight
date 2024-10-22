// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SAbilityBase.h"
#include "SAbility_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class ASAbility_Fireball : public ASAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteAbility() override;
	virtual void EndAbility() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Pegasus Dash")
	TSubclassOf<AActor> FireballActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pegasus Dash")
	class UAnimMontage* CancelAttackMontage;

};
