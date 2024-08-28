// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class USGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void CreateFog();

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	TSubclassOf<class ASFog> mFogClass;
};
