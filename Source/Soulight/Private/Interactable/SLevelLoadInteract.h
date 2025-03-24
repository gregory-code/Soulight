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
	
protected:
	UFUNCTION(BlueprintCallable)
	void LoadLevel();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Level Load")
	FName LevelName;

};
