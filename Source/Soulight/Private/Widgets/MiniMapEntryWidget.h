// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMiniMapEntryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetIcon(UTexture2D* IconTexture);
	void SetImageDesiredSizeOverride(const FVector2D& NewDesiredSize);
private:
	UPROPERTY(meta=(BindWidget))
	class UImage* IconImage;
};
