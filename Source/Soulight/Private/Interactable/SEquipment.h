// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SInteractableObject.h"
#include "SEquipment.generated.h"

/**
 * 
 */
UCLASS()
class ASEquipment : public ASInteractableObject
{
	GENERATED_BODY()
	
public:
	virtual void Interact() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	class USEquipmentData* Equipment;

};
