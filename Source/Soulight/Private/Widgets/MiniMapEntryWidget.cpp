// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MiniMapEntryWidget.h"
#include "Components/Image.h"

void UMiniMapEntryWidget::SetIcon(UTexture2D* IconTexture)
{
	IconImage->SetBrushFromTexture(IconTexture);
}

void UMiniMapEntryWidget::SetImageDesiredSizeOverride(const FVector2D& NewDesiredSize)
{
	IconImage->SetDesiredSizeOverride(NewDesiredSize);
}
