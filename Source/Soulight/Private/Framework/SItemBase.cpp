// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SItemBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Abilities/SAbilityDataBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Image.h"
#include "Engine/Texture.h"
#include "Widgets/SItemUI.h"
#include "Widgets/SItemUI.h"
#include "Engine/Texture2D.h"
#include "Widgets/SItemWidgetComponent.h"
#include "Player/SPlayer.h"
#include "Abilities/SAbilityBase.h"

// Sets default values
ASItemBase::ASItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GrabBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabBox"));
	RootComponent = GrabBox;
	FVector grabBoxRange = FVector(200, 200, 200);
	GrabBox->SetBoxExtent(grabBoxRange);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(GetRootComponent());
	//PickupMesh->SetSimulatePhysics(true);

	ItemWidgetComponent = CreateDefaultSubobject<USItemWidgetComponent>(TEXT("ItemComponent"));
	ItemWidgetComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ASItemBase::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ASItemBase::OnOverlapEnd);

	//ItemWidgetComponent->SetVisibility(false, false);

	ItemUI = Cast<USItemUI>(ItemWidgetComponent->GetWidget());
	if (ItemUI)
	{
		ItemUI->Start();
	}
}

// Called every frame
void ASItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ItemUI)
	{
		ItemUI->TickInRange(bInRange, DeltaTime);
	}
}

void ASItemBase::SetAbilityItem(USAbilityBase* ability, FString upgrade, FColor abilityColor)
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

void ASItemBase::Interact(bool bActionable)
{
	if (!bInRange) return;

	if (Player == nullptr || IsValid(AbilityItem) == false)
		return;

	Player->ObtainItem(AbilityItem);

	AbilityItem->SetOwner(Player);

	// hmm find a way to show this on the players HUD, and tell the player that they got an item. You'll be returning AbilityItem on this script

	this->Destroy();
}

void ASItemBase::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (Player == nullptr)
	{
		Player = Cast<ASPlayer>(otherActor);
	}

	if (!Player) return;
	if (Player != otherActor) return;
	
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

	Player->OnInteract.AddDynamic(this, &ASItemBase::Interact);

	bInRange = true;
	//ItemWidgetComponent->SetVisibility(true, false);
}

void ASItemBase::OnOverlapEnd(AActor* overlappedActor, AActor* otherActor)
{
	if (!Player) return;
	if (Player != otherActor) return;

	Player->OnInteract.RemoveDynamic(this, &ASItemBase::Interact);

	bInRange = false; 
	//ItemWidgetComponent->SetVisibility(false, false);
}

