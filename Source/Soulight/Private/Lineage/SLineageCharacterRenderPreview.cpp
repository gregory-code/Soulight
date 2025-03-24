// Fill out your copyright notice in the Description page of Project Settings.


#include "Lineage/SLineageCharacterRenderPreview.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"

ASLineageCharacterRenderPreview::ASLineageCharacterRenderPreview()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root Comp"));

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(RootComponent);

	HeadEquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadEquipmentMesh"));
	ChestEquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestEquipmentMesh"));
	BootsEquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BootsEquipmentMesh"));
	WeaponEquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponEquipmentMesh"));

	HeadEquipmentMesh->SetupAttachment(CharacterMesh);
	ChestEquipmentMesh->SetupAttachment(CharacterMesh);
	BootsEquipmentMesh->SetupAttachment(CharacterMesh);
	WeaponEquipmentMesh->SetupAttachment(CharacterMesh);

	ProfileCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	//SceneCaptureComponent->SetupAttachment(RootComponent);

	ProfileCaptureComponent->bCaptureEveryFrame = false;
	ProfileCaptureComponent->bCaptureOnMovement = false;
	ProfileCaptureComponent->SetupAttachment(GetRootComponent());
}

void ASLineageCharacterRenderPreview::UpdatePreviewEquipment(const EEquipmentType& EquipmentType, UStaticMesh* StaticMesh)
{
	switch (EquipmentType) 
	{
		case EEquipmentType::HEAD :
			HeadEquipmentMesh->SetStaticMesh(StaticMesh);
			break;
		case EEquipmentType::CHEST:
			ChestEquipmentMesh->SetStaticMesh(StaticMesh);
			break;
		case EEquipmentType::BOOTS :
			BootsEquipmentMesh->SetStaticMesh(StaticMesh);
			break;
		case EEquipmentType::WEAPON :
			WeaponEquipmentMesh->SetStaticMesh(StaticMesh);
			break;
	}
}

void ASLineageCharacterRenderPreview::CaptureSnapshot(UTextureRenderTarget2D* RenderTarget)
{
	if (!IsValid(RenderTarget)) return;

	ProfileCaptureComponent->TextureTarget = RenderTarget;

	ProfileCaptureComponent->CaptureScene();
}

UTextureRenderTarget2D* ASLineageCharacterRenderPreview::CreateNewRenderTargetTexture()
{
	UTextureRenderTarget2D* NewRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, 225, 350);
	return NewRenderTarget;
}

void ASLineageCharacterRenderPreview::ResetEquipmentDisplay()
{
	HeadEquipmentMesh->SetStaticMesh(nullptr);
	ChestEquipmentMesh->SetStaticMesh(nullptr);
	BootsEquipmentMesh->SetStaticMesh(nullptr);
	WeaponEquipmentMesh->SetStaticMesh(nullptr);
}

