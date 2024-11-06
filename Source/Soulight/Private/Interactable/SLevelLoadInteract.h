// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SInteractableObject.h"
#include "SLevelLoadInteract.generated.h"

/**
 * 
 */
UCLASS()
class ASLevelLoadInteract : public ASInteractableObject
{
	GENERATED_BODY()
	
public:
	virtual void Interact() override;

private:
	UFUNCTION()
	void LoadLevel();

	UPROPERTY(EditDefaultsOnly, Category = "Level Load")
	FName LevelName;

};
