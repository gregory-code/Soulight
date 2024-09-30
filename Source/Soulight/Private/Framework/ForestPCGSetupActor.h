// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForestPCGSetupActor.generated.h"

UCLASS()
class AForestPCGSetupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AForestPCGSetupActor();

protected:
	virtual void BeginPlay() override;

};
