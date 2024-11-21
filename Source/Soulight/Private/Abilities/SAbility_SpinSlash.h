// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SAbilityBase.h"
#include "SAbility_SpinSlash.generated.h"

/**
 * 
 */
UCLASS()
class ASAbility_SpinSlash : public ASAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteAbility() override;
	virtual void CancelAbility() override;
	virtual void EndAbility() override;

};
