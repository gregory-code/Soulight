// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SInteractableObject.h"
#include "SWritingTable.generated.h"

/**
 * 
 */
UCLASS()
class ASWritingTable : public ASInteractableObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BeginWrite(class ASPlayer* Player);

};
