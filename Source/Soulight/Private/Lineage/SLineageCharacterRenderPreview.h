// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Framework/SEquipmentData.h"
#include "SLineageCharacterRenderPreview.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class USceneCaptureComponent2D;

UCLASS()
class ASLineageCharacterRenderPreview : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLineageCharacterRenderPreview();

public:
	void UpdatePreviewEquipment(const EEquipmentType& EquipmentType, UStaticMesh* StaticMesh);
	void CaptureSnapshot(UTextureRenderTarget2D* RenderTarget);
	void ResetEquipmentDisplay();

	UTextureRenderTarget2D* CreateNewRenderTargetTexture();

private:
	UPROPERTY(VisibleAnywhere, Category = "Capture")
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(VisibleAnywhere, Category = "Capture")
	USceneCaptureComponent2D* ProfileCaptureComponent;

	UPROPERTY(VisibleAnywhere, Category = "Capture")
	UStaticMeshComponent* HeadEquipmentMesh;

	UPROPERTY(VisibleAnywhere, Category = "Capture")
	UStaticMeshComponent* ChestEquipmentMesh;

	UPROPERTY(VisibleAnywhere, Category = "Capture")
	UStaticMeshComponent* BootsEquipmentMesh;

	UPROPERTY(VisibleAnywhere, Category = "Capture")
	UStaticMeshComponent* WeaponEquipmentMesh;


};
