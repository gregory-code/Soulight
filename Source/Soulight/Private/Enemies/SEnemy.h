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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	TArray<TSubclassOf<class ASInteractableObject>> LootPool;

private:
	UFUNCTION()
	virtual void StartDeath(bool IsDead, AActor* DeadActor);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAIPerceptionStimuliSourceComponent* AIPerceptionSourceComp;

};