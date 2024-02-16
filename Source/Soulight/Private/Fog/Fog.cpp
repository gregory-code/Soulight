// Fill out your copyright notice in the Description page of Project Settings.


#include "Fog/Fog.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFog::AFog()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("Pickup Mesh");
	PickupMesh->SetupAttachment(GetRootComponent());
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = PickupMesh;
	//GrabBox = CreateDefaultSubobject<UBoxComponent>();
}

void AFog::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFog::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{

}

