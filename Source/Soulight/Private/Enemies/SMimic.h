// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/SEnemy.h"
#include "SMimic.generated.h"

/**
 * 
 */
UCLASS()
class ASMimic : public ASEnemy
{
	GENERATED_BODY()
	
private:
	virtual void StartDeath(bool IsDead) override;

	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	TArray<TSubclassOf<class ASInteractableObject>> EquipmentLootPool;

};
