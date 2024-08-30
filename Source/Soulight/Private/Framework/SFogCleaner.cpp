// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SFogCleaner.h"
#include "Framework/SFog.h"
#include "Components/SphereComponent.h"

// Sets default values
ASFogCleaner::ASFogCleaner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollider = CreateDefaultSubobject<USphereComponent>("Sphere Collider");
	RootComponent = SphereCollider;

	SetColliderRadius(600.0f);
	SphereCollider->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ASFogCleaner::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ASFogCleaner::OnOverlapBegin);
}

// Called every frame
void ASFogCleaner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASFogCleaner::SetColliderRadius(const float newRadius)
{
	SphereCollider->SetSphereRadius(newRadius);
}

void ASFogCleaner::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	ASFog* nearbyFog = Cast<ASFog>(otherActor);
	if (nearbyFog)
	{
		nearbyFog->Destroy();
	}
}

