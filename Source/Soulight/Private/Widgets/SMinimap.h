// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SMinimap.generated.h"

class UImage;
class UCanvasPanel;

USTRUCT(BlueprintType)
struct FMinimapEntry
{
	GENERATED_BODY()

	TWeakObjectPtr<AActor> Object;
	UTexture2D* IconTexture;
	FVector2D IconSize;
	FVector Location;

	FMinimapEntry()
		: Object(nullptr), IconTexture(nullptr), IconSize(FVector2D::ZeroVector), Location(FVector::ZeroVector)
	{}

	FMinimapEntry(AActor* InObject, UTexture2D* InIcon, const FVector2D& InSize, const FVector& InLocation)
		: Object(InObject), IconTexture(InIcon), IconSize(InSize), Location(InLocation) {}
};

/**
 * 
 */
UCLASS()
class USMinimap : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin Play or more compareable: Constructor
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateMinimap();

	
	UFUNCTION(BlueprintCallable)
	bool GetIsKnownObject(const AActor* TargetActor) const;

	UFUNCTION(BlueprintCallable)
	void SetOwningActor(AActor* OwningActor);

	UFUNCTION(BlueprintCallable)
	void SetPlayerController(APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable)
	void InitializeMinimap();

public:
	TSubclassOf<class UMiniMapEntryWidget> GetMiniMapEntryWidgetClass() const;
	
	UFUNCTION(BlueprintCallable)
	static void AddToMinimap(USMinimap* MinimapInstance, AActor* ObjectToAdd, UTexture2D* IconTexture, const FVector2D& IconSize, const FVector& Location);

	UFUNCTION(BlueprintCallable)
	static void RemoveFromMinimap(USMinimap* MinimapInstance, const AActor* ObjectToRemove);


protected:
	FVector2D ConvertWorldToMinimap(const FVector& WorldLocation, const FVector& PlayerLocation) const;
	FVector2D WorldToScreen(const FVector& WorldPosition, APlayerController* PlayerController) const;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MinimapCanvas;

	TMap<AActor*, class UMiniMapEntryWidget*> ChildIcons;

private:
	UPROPERTY(EditDefaultsOnly, Category = "MiniMap")
	TSubclassOf<class UMiniMapEntryWidget> MiniMapEntryWidgetClass;

	void AddPendingMiniMapEntries();

	static TArray<FMinimapEntry> PendingEntries;

	FVector MinimapCenter = FVector(0, 0, 0);

	float MinimapScale = 1000.0f;

	AActor* PlayerActor;

	class APlayerController* OwningPlayerController;
	class ACharacter* PlayerCharacter;

	const float MinimapWorldSize = 2048.0f;  // Adjust based on your game world
	const float MinimapSize = 200.0f;        // Your minimap size in pixels
	const float ScaleFactor = MinimapSize / MinimapWorldSize;

};
