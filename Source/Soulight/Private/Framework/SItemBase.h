// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SInteractableObject.h"
#include "SItemBase.generated.h"

class ASAbilityBase;

UCLASS()
class ASItemBase : public ASInteractableObject
{
	GENERATED_BODY()
	
public:	
	ASItemBase();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<ASAbilityBase> AbilityItemClass;

	ASAbilityBase* AbilityItem;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FColor AbilityUpgradeColor;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FColor AbilityNewColor;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FColor AbilityReplacesColor;

	UFUNCTION()
	void SetAbilityItem(class ASAbilityBase* ability, FString upgrade, FColor abilityColor);

public:	
	void Interact(bool bActionable) override;

	void OnOverlapBegin(AActor* overlappedActor, AActor* otherActor) override;
};
