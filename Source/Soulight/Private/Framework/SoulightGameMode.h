// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SoulightGameMode.generated.h"

class ASForestObjectiveActor;
/**
 * 
 */
UCLASS()
class ASoulightGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASForestObjectiveActor* GetForestObjectiveTracker() { return ForestObjectiveTracker; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Forest Level")
	TSubclassOf<ASForestObjectiveActor> ForestObjectiveTrackerClass;
	
	UPROPERTY()
	ASForestObjectiveActor* ForestObjectiveTracker;

};
