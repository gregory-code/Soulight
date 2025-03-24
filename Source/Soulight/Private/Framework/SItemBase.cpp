// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SItemBase.h"

#include "Abilities/SAbilityBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Image.h"

#include "Engine/World.h"
#include "Engine/Texture.h"
#include "Engine/Texture2D.h"

#include "Kismet/GameplayStatics.h"

#include "Player/Abilities/SAbilityDataBase.h"
#include "Player/SPlayer.h"

#include "Widgets/SItemUI.h"
#include "Widgets/SItemWidgetComponent.h"

ASItemBase::ASItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AbilityItemClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating Ability Item!"));

		AbilityItem = GetWorld()->SpawnActor<ASAbilityBase>(AbilityItemClass);
	}

	PlayerCharacter = Cast<ASPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ASItemBase::SetAbilityItem(ASAbilityBase* ability, FString upgrade, FColor abilityColor)
{
	if (ability == nullptr)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Setting"));

	USAbilityDataBase* AbilityData = ability->GetAbilityData();
	if (IsValid(AbilityData) == false) return;

	if (IsValid(ItemUI))
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting For Realsies"));

		ItemUI->SetItem(AbilityData->GetAbilityName(), upgrade, AbilityData->GetAbilityIcon(), abilityColor);
	}
}

void ASItemBase::EnableInteractionWidget_Implementation()
{
	Super::EnableInteractionWidget_Implementation(); // Call parent to handle widget visibility

	if (IsValid(ItemUI) && IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("ASItemBase: Interaction Widget Enabled for %s"), *GetName());
		ShowItemStatus(PlayerCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASItemBase: ItemUI or PlayerCharacter not valid for %s"), *GetName());
	}
}

void ASItemBase::DisableInteractionWidget_Implementation()
{
	Super::DisableInteractionWidget_Implementation(); // Call parent to handle widget visibility

	UE_LOG(LogTemp, Warning, TEXT("ASItemBase: Interaction Widget Disabled for %s"), *GetName());
}

void ASItemBase::ObtainItem(class ASPlayer* Player)
{
	if (Player == nullptr || IsValid(AbilityItem) == false)
		return;

	Player->ObtainItem(AbilityItem);

	AbilityItem->RegisterAbility(Player);

	// hmm find a way to show this on the players HUD, and tell the player that they got an item. You'll be returning AbilityItem on this script

	this->Destroy();
}

void ASItemBase::ShowItemStatus(class ASPlayer* Player)
{
	if (!IsValid(AbilityItem) || !IsValid(Player)) return;

	switch (Player->GetItemStatus(AbilityItem, Player->GetItemTypeFromNew(AbilityItem)))
	{
		case EUpgrade::New:
			SetAbilityItem(AbilityItem, FString("- New -"), AbilityNewColor);
			break;

		case EUpgrade::Upgrade:
			SetAbilityItem(AbilityItem, FString("Upgrade +++"), AbilityUpgradeColor);
			break;

		case EUpgrade::Replace:
			SetAbilityItem(AbilityItem, FString("Replaces -> "), AbilityReplacesColor);
			break;
	}
}

UTexture* ASItemBase::GetAbilityIcon() const
{
	if (IsValid(AbilityItem))
	{
		return AbilityItem->GetAbilityData()->GetAbilityIcon();
	}

	return nullptr;
}

FString ASItemBase::GetAbilityName() const
{
	if (IsValid(AbilityItem))
	{
		return AbilityItem->GetAbilityName();
	}

	return "";
}

