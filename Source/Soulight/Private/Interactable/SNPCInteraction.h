// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SInteractableObject.h"
#include "SNPCInteraction.generated.h"

/**
 * 
 */
UCLASS()
class ASNPCInteraction : public ASInteractableObject
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact() override;

	virtual void OnOverlapBegin(AActor* overlappedActor, AActor* otherActor) override;

	virtual void OnOverlapEnd(AActor* overlappedActor, AActor* otherActor) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> NPCGUIPopupClass;

	class UUserWidget* NPCGUIPopup;

};
