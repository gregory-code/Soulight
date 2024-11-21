// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SAbilityBase.h"
#include "SAbility_OverheadCrush.generated.h"

/**
 * 
 */
UCLASS()
class ASAbility_OverheadCrush : public ASAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteAbility() override;
	virtual void CancelAbility() override;
	virtual void EndAbility() override;

};
