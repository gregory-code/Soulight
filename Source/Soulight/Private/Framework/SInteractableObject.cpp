// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SInteractableObject.h"

#include "Abilities/SAbilityBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Image.h"

#include "Engine/World.h"
#include "Engine/Texture.h"
#include "Engine/Texture2D.h"
#include "Engine/EngineTypes.h"

#include "Kismet/GameplayStatics.h"

#include "Player/Abilities/SAbilityDataBase.h"
#include "Player/SPlayer.h"
#include "Widgets/SItemUI.h"
#include "Widgets/SItemWidgetComponent.h"

ASInteractableObject::ASInteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	RootCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Root Box Collider"));
	SetRootComponent(RootCollider);

	// Set Collision Object Type to Minimap
	RootCollider->SetCollisionObjectType(ECC_GameTraceChannel3);

	// Block everything by default
	RootCollider->SetCollisionResponseToAllChannels(ECR_Block);

	// Ignore Custom Channel 1 & 2
	RootCollider->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	RootCollider->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootCollider);

	ItemWidgetComponent = CreateDefaultSubobject<USItemWidgetComponent>(TEXT("ItemComponent"));
	ItemWidgetComponent->SetupAttachment(RootCollider);
}

void ASInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(ItemWidgetComponent->GetWidget())) return;

	ItemUI = Cast<USItemUI>(ItemWidgetComponent->GetWidget());
	if (ItemUI)
	{
		ItemUI->Start();
	}
	else if (IsValid(ItemWidgetComponent->GetWidget()))
	{
		ItemWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ASInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(ItemUI))
	{
		return;
	}

	ItemUI->TickInRange(bInRange, DeltaTime);
}

void ASInteractableObject::Interact_Implementation(class ASPlayer* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("I AM INTERACTING!"));

	UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(InteractMontage, 1.0f);
	}

	IIInteractable::Execute_OnInteract(this, Player);
}

void ASInteractableObject::EnableInteractionWidget_Implementation()
{
	if (IsValid(ItemWidgetComponent->GetWidget()))
	{
		ItemWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	}

	bInRange = true;

	IIInteractable::Execute_OnWidgetEnable(this);

}

void ASInteractableObject::DisableInteractionWidget_Implementation()
{
	if (IsValid(ItemWidgetComponent->GetWidget()))
	{
		ItemWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}

	bInRange = false;

	IIInteractable::Execute_OnWidgetDisable(this);
}
