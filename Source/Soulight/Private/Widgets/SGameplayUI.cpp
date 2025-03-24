// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SGameplayUI.h"

#include "Components/Image.h"
#include "Components/StackBox.h"

#include "Engine/Texture2D.h"

#include "Widgets/SMinimap.h"
#include "Widgets/SPickupPopup.h"

#include "Player/SPlayer.h"
#include "PaperSprite.h"

void USGameplayUI::NativeConstruct()
{
	Super::NativeConstruct();

	Minimap->InitializeMinimap();
}

void USGameplayUI::InitGameplayUI(ASPlayer* Player)
{
	if (!IsValid(Player)) return;

	UE_LOG(LogTemp, Warning, TEXT("Initializing GameplayUI"));
	Player->OnPickup.AddDynamic(this, &USGameplayUI::AddPickupEntry);
	Player->OnPickupEquipment.AddDynamic(this, &USGameplayUI::AddPickupEntryEquipment);
}

void USGameplayUI::SetSkillImage(UTexture* targetImage)
{
	if (!IsValid(targetImage)) return;

	if (!IsValid(SkillImage)) return;

	SkillImage->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParameterName, targetImage);
}

void USGameplayUI::SetSpellImage(UTexture* targetImage)
{
	if (!IsValid(targetImage)) return;

	if (!IsValid(SpellImage)) return;

	SpellImage->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParameterName, targetImage);
}

void USGameplayUI::SetHealth(float Current, float Max)
{
	if (LampHealthImage)
	{
		float Opacity = Current / Max;

		FLinearColor NewColor = LampHealthImage->GetBrush().TintColor.GetSpecifiedColor();
		NewColor.A = Opacity;

		LampHealthImage->SetColorAndOpacity(NewColor);
	}
}

void USGameplayUI::AddPickupEntry(UTexture* PickupIconTexture, FString PickupItemName)
{
	if (!IsValid(PickupPopupClass)) return;

	UE_LOG(LogTemp, Warning, TEXT("Creating Item UI Popup"));

	USPickupPopup* NewPickupEntry = CreateWidget<USPickupPopup>(this, PickupPopupClass);
	NewPickupEntry->AddToViewport();
	NewPickupEntry->SetRenderScale(FVector2D(0.8f, 0.8f));

	PickupStack->AddChild(NewPickupEntry);

	//NewPickupEntry->SetPositionInViewport(FVector2D::ZeroVector, true);

	NewPickupEntry->InitIcon(PickupIconTexture, PickupItemName);

	PickupQueue.Enqueue(NewPickupEntry);

	if (!GetWorld()->GetTimerManager().IsTimerActive(ClearPickupHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(ClearPickupHandle, this, &USGameplayUI::StartClearTimer, 1.f / 24.f, true);
	}
}

void USGameplayUI::AddPickupEntryEquipment(UPaperSprite* PickupIconTexture, FString PickupItemName)
{
	if (!IsValid(PickupPopupClass)) return;

	UE_LOG(LogTemp, Warning, TEXT("Creating Item UI Popup"));

	USPickupPopup* NewPickupEntry = CreateWidget<USPickupPopup>(this, PickupPopupClass);
	NewPickupEntry->AddToViewport();
	NewPickupEntry->SetRenderScale(FVector2D(0.8f, 0.8f));

	PickupStack->AddChild(NewPickupEntry);

	NewPickupEntry->InitIconSprite(PickupIconTexture, PickupItemName);

	PickupQueue.Enqueue(NewPickupEntry);

	if (!GetWorld()->GetTimerManager().IsTimerActive(ClearPickupHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(ClearPickupHandle, this, &USGameplayUI::StartClearTimer, 1.f / 24.f, true);
	}
}

void USGameplayUI::StartClearTimer()
{
	if (PickupQueue.IsEmpty()) 
	{
		GetWorld()->GetTimerManager().ClearTimer(ClearPickupHandle);
		return;
	}

	float IncrementRate = 0.1f;
	CurrentTime += IncrementRate;

	if (CurrentTime >= ClearTime)
	{
		RemovePickup();
		CurrentTime = 0;
	}
}

void USGameplayUI::RemovePickup()
{
	USPickupPopup* RemovedPickup = nullptr;
	if (PickupQueue.Dequeue(RemovedPickup) && IsValid(RemovedPickup))
	{
		RemovedPickup->RemoveFromParent();
	}
}
