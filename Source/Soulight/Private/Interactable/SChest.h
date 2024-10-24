// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SInteractableObject.h"
#include "SChest.generated.h"

/**
 * 
 */
UCLASS()
class ASChest : public ASInteractableObject
{
	GENERATED_BODY()
	
public:
	virtual void Interact() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Chest")
	TArray<TSubclassOf<ASInteractableObject>> ItemPool;

};
