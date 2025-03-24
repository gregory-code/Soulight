// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameplayUI.generated.h"

class USMinimap;
class USPickupPopup;
/**
 * 
 */
UCLASS()
class USGameplayUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Begin Play or more compareable: Constructor
	virtual void NativeConstruct() override;

	void InitGameplayUI(class ASPlayer* Player);

	void SetSkillImage(UTexture* targetImage);
	void SetSpellImage(UTexture* targetImage);

	UFUNCTION()
	void SetHealth(float Current, float Max);

	UFUNCTION(BlueprintCallable)
	USMinimap* GetMinimap() const { return Minimap; }
	
	UFUNCTION()
	void AddPickupEntry(UTexture* PickupIconTexture, FString PickupItemName);

	UFUNCTION()
	void AddPickupEntryEquipment(class UPaperSprite* PickupIconTexture, FString PickupItemName);


private:
	UPROPERTY(EditDefaultsOnly)
		FName IconMaterialParameterName {
		"Icon"
	};

	UPROPERTY(meta = (BindWidget))
	USMinimap* Minimap;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* SpellImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* LampHealthImage;

	UPROPERTY(meta = (BindWidget))
	class UStackBox* PickupStack;

	UPROPERTY(EditAnywhere, Category = "Pickup Popup")
	TSubclassOf<USPickupPopup> PickupPopupClass;

	UFUNCTION()
	void StartClearTimer();

	void RemovePickup();

	TQueue<USPickupPopup*> PickupQueue;
	FTimerHandle ClearPickupHandle;

	UPROPERTY(EditAnywhere, Category = "Pickup Popup")
	float ClearTime = 3.f;
	float CurrentTime = 0.0f;

};
