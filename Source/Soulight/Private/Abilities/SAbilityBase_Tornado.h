// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SAbilityBase.h"
#include "SAbilityBase_Tornado.generated.h"

/**
 * 
 */
UCLASS()
class ASAbilityBase_Tornado : public ASAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteAbility() override;
	virtual void CancelAbility() override;
	virtual void EndAbility() override;

private:
	void StartTornado();

};
