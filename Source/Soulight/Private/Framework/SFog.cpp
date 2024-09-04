// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SFog.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
ASFog::ASFog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VolumeFogComponent = CreateDefaultSubobject<UStaticMeshComponent>("FogComponent");
	VolumeFogComponent->SetWorldScale3D(FVector(8.0f, 8.0f, 10.0f));

	RootComponent = VolumeFogComponent;

	VolumeFogComponent->SetAutoActivate(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	if (SphereMeshAsset.Succeeded())
	{
		VolumeFogComponent->SetStaticMesh(SphereMeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ASFog::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void ASFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASFog::IsCursedFog()
{
	return bCursedFog;
}

