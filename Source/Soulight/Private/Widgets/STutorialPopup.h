// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STutorialPopup.generated.h"

class ASPlayer;
/**
 * 
 */
UCLASS()
class USTutorialPopup : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Init(ASPlayer* Player);

protected:
	UFUNCTION(BlueprintCallable)
	void EnablePlayerControls();

	UFUNCTION(BlueprintCallable)
	ASPlayer* GetPlayer() const { return OwningPlayer; }

private:
	ASPlayer* OwningPlayer;

};
