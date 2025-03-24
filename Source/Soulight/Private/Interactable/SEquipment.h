// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SInteractableObject.h"
#include "SEquipment.generated.h"

class USEquipmentData;
/**
 * 
 */
UCLASS()
class ASEquipment : public ASInteractableObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void EquipItem(class ASPlayer* Player);

	USEquipmentData* GetEquipmentData() const { return Equipment; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	USEquipmentData* Equipment;

};
