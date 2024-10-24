// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SInteractableObject.h"
#include "SLamppost.generated.h"

/**
 * 
 */
UCLASS()
class ASLamppost : public ASInteractableObject
{
	GENERATED_BODY()

public:
	ASLamppost();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Fog Cleaner")
	TSubclassOf<class ASFogCleaner> FogCleanerClass;

	UPROPERTY(visibleAnywhere, Category = "Fog Cleaner")
	class ASFogCleaner* FogCleaner;

	UPROPERTY()
	class ASoulightGameMode* SoulightGameMode;

	UPROPERTY(EditAnywhere, Category = "Light")
	class UPointLightComponent* PointLight;

public:
	virtual void Interact() override;

};
