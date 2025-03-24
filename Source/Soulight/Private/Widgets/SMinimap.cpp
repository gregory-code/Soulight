// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/SMinimap.h"

#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

#include "UObject/ConstructorHelpers.h"

#include "Widgets/MiniMapEntryWidget.h"

TArray<FMinimapEntry> USMinimap::PendingEntries = TArray<FMinimapEntry>();

void USMinimap::NativeConstruct()
{
	Super::NativeConstruct();

	MinimapCenter = FVector(0, 0, 0); 
	MinimapScale = 1.0f;
	OwningPlayerController = GetOwningPlayer<APlayerController>();
}

void USMinimap::UpdateMinimap()
{
	if (!OwningPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Update Minimap, Player Controller Is Missing Reference"));
		return;
	}

	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Update Minimap, Player Character Is Missing Reference"));
		return;
	}

	AddPendingMiniMapEntries();

	if (!MinimapCanvas)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Update Minimap, MinimapCanvas Is Missing Reference"));
		return;
	}

	if (ChildIcons.IsEmpty()) return;

	float MinimapWidth = 200.0f;
	float MinimapHeight = 200.0f;
	float HalfWidth = MinimapWidth * 0.5f;
	float HalfHeight = MinimapHeight * 0.5f;

	FVector PlayerWorldPos = PlayerCharacter->GetActorLocation();

	for (const auto& Entry : ChildIcons)
	{
		AActor* Object = Entry.Key;
		UMiniMapEntryWidget* IconImage = Entry.Value;

		if (Object && IconImage)
		{
			// It;s crashing here, 'Object' is likely going null and not 
			// clearing itself from 'ChildIcons' List!
			FVector ObjectWorldPos = Object->GetActorLocation(); 
			FVector RelativePosition = ObjectWorldPos - PlayerWorldPos;

			FVector2D MinimapPosition = FVector2D(
				100.0f + (RelativePosition.Y * ScaleFactor),
				100.0f - (RelativePosition.X * ScaleFactor)
			);

			MinimapPosition.X = FMath::Clamp(MinimapPosition.X, 0.0f, MinimapWidth);
			MinimapPosition.Y = FMath::Clamp(MinimapPosition.Y, 0.0f, MinimapHeight);

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(IconImage->Slot);
			if (CanvasSlot)
			{
				CanvasSlot->SetPosition(MinimapPosition);
			}
		}
	}
}

TSubclassOf<class UMiniMapEntryWidget> USMinimap::GetMiniMapEntryWidgetClass() const
{
	return MiniMapEntryWidgetClass;
}

void USMinimap::AddToMinimap(USMinimap* MinimapInstance, AActor* ObjectToAdd, UTexture2D* IconTexture, const FVector2D& IconSize, const FVector& Location)
{
	if (!IsValid(MinimapInstance) || !IsValid(ObjectToAdd) || !IsValid(IconTexture))
	{
		return;
	}

	PendingEntries.Add(FMinimapEntry(ObjectToAdd, IconTexture, IconSize, Location));	
}

void USMinimap::RemoveFromMinimap(USMinimap* MinimapInstance, const AActor* ObjectToRemove)
{
	if (!MinimapInstance || !ObjectToRemove || !MinimapInstance->ChildIcons.Contains(ObjectToRemove))
	{
		return; // Prevent unnecessary operations
	}

	if (auto* IconWidget = MinimapInstance->ChildIcons[ObjectToRemove])
	{
		IconWidget->RemoveFromParent();
	}

	MinimapInstance->ChildIcons.Remove(ObjectToRemove);
}

bool USMinimap::GetIsKnownObject(const AActor* TargetActor) const 
{
	return ChildIcons.Contains(TargetActor);
}

void USMinimap::SetOwningActor(AActor* OwningActor)
{
	PlayerActor = OwningActor;
}

void USMinimap::SetPlayerController(APlayerController* PlayerController)
{
	OwningPlayerController = PlayerController;

	PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());

}

void USMinimap::InitializeMinimap()
{
	if (!IsValid(this) || !IsValid(MinimapCanvas)) return;

	for (const FMinimapEntry& Entry : PendingEntries)
	{
		if (Entry.Object.IsValid())
		{
			AddToMinimap(this, Entry.Object.Get(), Entry.IconTexture, Entry.IconSize, Entry.Location);
		}
	}

	PendingEntries.Empty();
}

FVector2D USMinimap::ConvertWorldToMinimap(const FVector& WorldLocation, const FVector& PlayerLocation) const
{
	/*
	// Calculate the offset from the player position
	FVector Offset = WorldLocation - PlayerLocation;

	// Get minimap size
	FVector2D MinimapSize = MinimapCanvas->GetDesiredSize();

	// Scale factor to map world coordinates to minimap (adjust MinimapScale for better scaling)
	//float ScaleFactor = 1.0f / MinimapScale;
	float ScaleFactor = 0.05f;

	// Convert world offset to minimap space
	FVector2D MinimapPos;

	// Adjust for world-to-minimap conversion
	MinimapPos.X = (Offset.Y * ScaleFactor) + (MinimapSize.Y * 0.5f);  // X (horizontal) scale
	MinimapPos.Y = (-Offset.X * ScaleFactor) + (MinimapSize.X * 0.5f); // Y (vertical) scale and inversion

	// Ensure the position is within the minimap bounds
	MinimapPos.X = FMath::Clamp(MinimapPos.X, -MinimapSize.X, MinimapSize.X);
	MinimapPos.Y = FMath::Clamp(MinimapPos.Y, -MinimapSize.Y, MinimapSize.Y);

	// Return the final minimap position
	return MinimapPos;
	*/
	return FVector2D::ZeroVector;
}

FVector2D USMinimap::WorldToScreen(const FVector& WorldPosition, APlayerController* PlayerController) const
{
	FVector2D ScreenPosition;

	// Convert world position to screen position
	bool CastSucceded = PlayerController->ProjectWorldLocationToScreen(WorldPosition, ScreenPosition);
	if (!CastSucceded) 
	{
		UE_LOG(LogTemp, Log, TEXT("Could not project to world location to screen"));
	}

	return ScreenPosition;
}

void USMinimap::AddPendingMiniMapEntries()
{
	if (PendingEntries.Num() == 0)
	{
		return;
	}

	if (!OwningPlayerController || !GetMiniMapEntryWidgetClass())
	{
		return;
	}

	for (const FMinimapEntry& MinimapEntry : PendingEntries)
	{
		if (!MinimapEntry.Object.IsValid())
			continue;

		UMiniMapEntryWidget* NewEntryWidget = CreateWidget<UMiniMapEntryWidget>(OwningPlayerController, GetMiniMapEntryWidgetClass());
		NewEntryWidget->SetIcon(MinimapEntry.IconTexture);
		NewEntryWidget->SetImageDesiredSizeOverride(MinimapEntry.IconSize);
		if (UCanvasPanelSlot* NewEntryPanelSlot = MinimapCanvas->AddChildToCanvas(NewEntryWidget))
		{
			NewEntryPanelSlot->SetSize(MinimapEntry.IconSize);
			NewEntryPanelSlot->SetAlignment(FVector2D{ 0.5f });
			ChildIcons.Add(MinimapEntry.Object.Get(), NewEntryWidget);
		}
	}

	PendingEntries.Empty();
}
