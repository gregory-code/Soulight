// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLineageAnimationPanel.generated.h"

class UTexture2D;
/**
 * 
 */
UCLASS()
class USLineageAnimationPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitLineageEntries();

private:
	UTexture2D* GetAbilityIcon(const FString& AbilityName) const;

	class USGameInstance* GameInstance;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* EntryBoxContainer;

	TArray<class USLineageEntry*> EntryPanels;

	float ElapsedTime = -0.5f;

};
