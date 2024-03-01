// Fill out your copyright notice in the Description page of Project Settings.


#include "Fog/Fog.h"
#include "Components/BoxComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFog::AFog()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("Pickup Mesh");
	PickupMesh->SetupAttachment(GetRootComponent());
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = PickupMesh;
	GrabBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabBox"));
	GrabBox->SetBoxExtent(FVector(10, 10, 10));
	GrabBox->AttachToComponent(PickupMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void AFog::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Overlapped with an actor"));

	OnActorBeginOverlap.AddDynamic(this, &AFog::OnOverlapBegin);
}

// Called every frame
void AFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFog::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with an actor"));

	APlayerCharacter* player = Cast<APlayerCharacter>(otherActor);
	if (!player)
	{
		return;
	}

	Destroy();

}

