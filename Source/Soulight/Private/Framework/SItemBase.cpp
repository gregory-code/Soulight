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
}

void ASItemBase::SetAbilityItem(ASAbilityBase* ability, FString upgrade, FColor abilityColor)
{
	if (ability == nullptr)
		return;

	USAbilityDataBase* AbilityData = ability->GetAbilityData();
	if (IsValid(AbilityData) == false) return;

	USItemUI* itemUI = Cast<USItemUI>(ItemWidgetComponent->GetWidget());
	if (itemUI)
	{
		itemUI->SetItem(AbilityData->GetAbilityName(), upgrade, AbilityData->GetAbilityIcon(), abilityColor);
	}
}

void ASItemBase::Interact()
{
	Super::Interact();

	if (Player == nullptr || IsValid(AbilityItem) == false)
		return;

	Player->ObtainItem(AbilityItem);

	AbilityItem->SetAbilityOwner(Player);

	// hmm find a way to show this on the players HUD, and tell the player that they got an item. You'll be returning AbilityItem on this script

	this->Destroy();
}

void ASItemBase::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	Super::OnOverlapBegin(overlappedActor, otherActor);

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

