// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SInteractableObject.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

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
	
}

// Called every frame
void ASInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASInteractableObject::Interact(AActor* InstigatingObject)
{
	
}

