// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STutorialComponent.generated.h"

class USTutorialPopup;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USTutorialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTutorialComponent();

protected:
	virtual void BeginPlay() override;

public:	
	void EvaluateObject(AActor* ObjectToEvaluate);
		
	// I do need a whole bunch of widgets
	// use the scalable approach widget has one image one text
private:
	class USGameInstance* SGameInstance;

	class APlayerController* OwningController;
	class ASPlayer* Player;

	void ShowTutorialPopup(TSubclassOf<USTutorialPopup> TutorialPopupClass /* Change this to a custom hint widget class */);

	UPROPERTY(EditDefaultsOnly, Category = "Tutorial")
	TMap<UClass*, TSubclassOf<USTutorialPopup>> TutorialWidgets;
};
