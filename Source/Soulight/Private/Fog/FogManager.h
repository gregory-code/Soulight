// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogManager.generated.h"

UCLASS()
class AFogManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AFogManager();

	UPROPERTY()
	TArray<class AFog*> FogArray;

	UPROPERTY(EditAnywhere, Category = "Fog")
	class AFog* FogClass;

	UPROPERTY(EditAnywhere, Category = "Fog")
	int GridSizeX;

	UPROPERTY(EditAnywhere, Category = "Fog")
	int GridSizeY;

	UPROPERTY(EditAnywhere, Category = "Fog")
	int CellSize;

	UPROPERTY(EditAnywhere, Category = "Fog")
	float MinScale;

	UPROPERTY(EditAnywhere, Category = "Fog")
	float MaxScale;

protected:

	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void SpawnClouds();

public:	

	virtual void Tick(float DeltaTime) override;



};
