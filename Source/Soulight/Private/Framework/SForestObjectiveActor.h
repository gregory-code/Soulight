// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SForestObjectiveActor.generated.h"

UCLASS()
class ASForestObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASForestObjectiveActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Forest Level")
	int32 TargetLampCount = 3;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Forest Level")
	FName CastleLevelName;

	int32 ActiveLampCount = 0;

};
