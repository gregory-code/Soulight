// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SDungeonGenerationComponent.h"

#include "Engine/World.h"
#include "EngineUtils.h"

#include "Framework/SDungeonRoom.h"

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
	
	GenerateDungeon(MaxNumRoom);

    GenerateChests(MaxNumChests);
}


// Called every frame
void USDungeonGenerationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USDungeonGenerationComponent::DepthFirstSearch(ASDungeonRoom* CurrentRoom, ASDungeonRoom* EndRoom, int32& RoomCount, const int32& NumRooms)
{
    if (RoomCount >= NumRooms || CurrentRoom == EndRoom) return;

    CurrentRoom->bVisited = true;

    FVector ToEndRoomDirection = EndRoom->GetActorLocation() - CurrentRoom->GetActorLocation();
    ToEndRoomDirection.Normalize();

    TArray<FVector> RoomDirections;
    RoomDirections.Append(TryGetPossibleDirections(CurrentRoom));

    if (RoomDirections.Num() == 0) return;

    RoomDirections.Sort([&](const FVector& A, const FVector& B) {
        return FVector::DotProduct(A, ToEndRoomDirection) > FVector::DotProduct(B, ToEndRoomDirection);
        });

    for (const FVector& Direction : RoomDirections)
    {
        FVector NewRoomPosition = CurrentRoom->GetActorLocation() + Direction;

        if (!IsRoomAlreadyGenerated(NewRoomPosition))
        {
            ASDungeonRoom* NewRoom = GenerateRoom(StartingRoomClass, NewRoomPosition);

            if (NewRoom)
            {
                CurrentRoom->AddChildRoom(NewRoom);

                RoomCount++;

                if (NewRoom == EndRoom)
                {
                    return; 
                }

                DepthFirstSearch(NewRoom, EndRoom, RoomCount, NumRooms);
            }
        }

        if (RoomCount >= NumRooms) break;
    }
}

#pragma region Procedural Generation Functions

void USDungeonGenerationComponent::GenerateDungeon(const int32& NumRooms)
{
    if (StartingRoomClass == nullptr || BossRoomClass == nullptr) return;

    FVector StartRoomPos = FVector(0, 0, -100.0f);
    ASDungeonRoom* StartRoom = GenerateRoom(StartingRoomClass, StartRoomPos);

    FVector BossRoomPos = FVector(9000.0f, 9000.0f, -100.0f);
    ASDungeonRoom* BossRoom = GenerateRoom(BossRoomClass, BossRoomPos);

    int32 RoomCount = 0;
    DepthFirstSearch(StartRoom, BossRoom, RoomCount, NumRooms);

    if (!BossRoom->bVisited)
    {
        StartRoom->AddChildRoom(BossRoom);
    }

}

ASDungeonRoom* USDungeonGenerationComponent::GenerateRoom(TSubclassOf<ASDungeonRoom> RoomClass, const FVector& Position)
{
	if (RoomClass == nullptr) return nullptr;

	FActorSpawnParameters SpawnParams;
	ASDungeonRoom* InstancedRoom = GetWorld()->SpawnActor<ASDungeonRoom>(RoomClass, Position, FRotator::ZeroRotator, SpawnParams);
	if (InstancedRoom) 
	{
		UE_LOG(LogTemp, Log, TEXT("Room spawned at position: %s"), *Position.ToString());

        if(InstancedRoom->GetChestSpawnPoints().Num() > 0)
            ChestSpawnPoints.Append(InstancedRoom->GetChestSpawnPoints());

		return InstancedRoom;
	}

	return nullptr;
}

void USDungeonGenerationComponent::GenerateChests(const int32& NumChests)
{
    if (ChestClass == nullptr) return;

    if (NumChests == 0 && ChestSpawnPoints.Num() == 0) return;

    for (int i = NumChests;  i > 0; i--) 
    {
        if (ChestSpawnPoints.Num() == 0) return;

        int32 RandomIndex = FMath::RandRange(0, ChestSpawnPoints.Num() - 1);
        
        FActorSpawnParameters SpawnParams;
        AActor* InstancedChest = GetWorld()->SpawnActor<AActor>
            (
                ChestClass, 
                ChestSpawnPoints[RandomIndex].GetLocation(),
                ChestSpawnPoints[RandomIndex].Rotator(),
                SpawnParams
            );

        ChestSpawnPoints.RemoveAt(RandomIndex);
    }
}

#pragma endregion

#pragma region Helper Functions

bool USDungeonGenerationComponent::IsRoomAlreadyGenerated(const FVector& RoomPosition)
{
    for (TActorIterator<ASDungeonRoom> It(GetWorld()); It; ++It)
    {
        if (It->GetActorLocation().Equals(RoomPosition, 0.1f))
        {
            return true;
        }
    }

    return false;
}

TArray<FVector> USDungeonGenerationComponent::TryGetPossibleDirections(ASDungeonRoom* CurrentRoom)
{
    TArray<FVector> Directions;

    if (CurrentRoom->GetUpOpening())
        Directions.Add(PossibleDirections[0]);
    if (CurrentRoom->GetDownOpening())
        Directions.Add(PossibleDirections[1]);
    if (CurrentRoom->GetRightOpening())
        Directions.Add(PossibleDirections[2]);
    if (CurrentRoom->GetLeftOpening())
        Directions.Add(PossibleDirections[3]);

    return Directions;
}

#pragma endregion