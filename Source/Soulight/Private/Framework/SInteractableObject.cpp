// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SInteractableObject.h"

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

ASInteractableObject::ASInteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	GrabBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabBox"));
	RootComponent = GrabBox;
	FVector grabBoxRange = FVector(200, 200, 200);
	GrabBox->SetBoxExtent(grabBoxRange);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(GetRootComponent());

	ItemWidgetComponent = CreateDefaultSubobject<USItemWidgetComponent>(TEXT("ItemComponent"));
	ItemWidgetComponent->SetupAttachment(GetRootComponent());
}

void ASInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	ItemUI = Cast<USItemUI>(ItemWidgetComponent->GetWidget());
	if (ItemUI)
	{
		ItemUI->Start();
	}
	else if (IsValid(ItemWidgetComponent->GetWidget()))
	{
		ItemWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}


	OnActorBeginOverlap.AddDynamic(this, &ASInteractableObject::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ASInteractableObject::OnOverlapEnd);
}

void ASInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ItemUI)
	{
		ItemUI->TickInRange(bInRange, DeltaTime);
	}
}

void ASInteractableObject::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("I AM INTERACTING!"));

	if (!IsValid(Player)) return;

	Player->OnInteract.RemoveDynamic(this, &ASInteractableObject::Interact);
	OnActorBeginOverlap.RemoveDynamic(this, &ASInteractableObject::OnOverlapBegin);

	if (!IsValid(Player->GetMesh()) || !IsValid(InteractMontage)) return;

	if (!IsValid(Player->GetMesh()->GetAnimInstance())) return;

	UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(InteractMontage, 1.0f);
	}
}

void ASInteractableObject::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (!IsValid(Player))
		Player = Cast<ASPlayer>(otherActor);

	if (!IsValid(Player) || Player != otherActor) return;

	if (!IsValid(ItemUI) && IsValid(ItemWidgetComponent->GetWidget()))
	{
		ItemWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	}

	Player->OnInteract.RemoveDynamic(this, &ASInteractableObject::Interact);
	Player->OnInteract.AddDynamic(this, &ASInteractableObject::Interact);

	bInRange = true;
}

void ASInteractableObject::OnOverlapEnd(AActor* overlappedActor, AActor* otherActor)
{
	if (!IsValid(Player)) return;

	if (Player != otherActor) return;

	if (!IsValid(ItemUI) && IsValid(ItemWidgetComponent->GetWidget()))
	{
		ItemWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}

	Player->OnInteract.RemoveDynamic(this, &ASInteractableObject::Interact);

	bInRange = false;
}

