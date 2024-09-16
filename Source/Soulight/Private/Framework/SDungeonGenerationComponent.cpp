// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SDungeonGenerationComponent.h"

#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USDungeonGenerationComponent::USDungeonGenerationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USDungeonGenerationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	GenerateDungeon(5);
}


// Called every frame
void USDungeonGenerationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USDungeonGenerationComponent::GenerateDungeon(const int32& NumRooms)
{
	// Generate The Starting Room
	//GenerateRoom(StartingRoom, FVector(0,0, -750.0f));

	//GenerateRoom(BossRoom, FVector(9000.0f, 9000.0f, -750.0f));

	//if (NumRooms == 0) return;

	for (int i = 0; i < GridSize.X; i++) 
	{
		for (int j = 0; j < GridSize.Y; j++) 
		{
			GenerateRoom(StartingRoom, FVector(i * GridOffset, j * GridOffset, -1500.0f));
		}
	}

}

void USDungeonGenerationComponent::GenerateRoom(TSubclassOf<AStaticMeshActor> Room, const FVector& Position)
{
	if (Room == nullptr) return;

	FActorSpawnParameters SpawnParams;
	AStaticMeshActor* InstancedRoom = GetWorld()->SpawnActor<AStaticMeshActor>(Room, Position, FRotator::ZeroRotator, SpawnParams);
	if (InstancedRoom) 
	{
		// Do Something, prolly check for spawn points and spawn other shit
	}

}

