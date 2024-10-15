// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SCharacterBase.h"
#include "SEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ASEnemy : public ASCharacterBase
{
	GENERATED_BODY()
	
public:
	ASEnemy();

private:
	UFUNCTION()
	void StartDeath(bool IsDead);

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAIPerceptionStimuliSourceComponent* AIPerceptionSourceComp;

};
