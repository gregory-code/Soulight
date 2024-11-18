// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/DungeonGenerator.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

    /*
    InitializeGrid();

    PlaceStartRoom();
    PlaceBossRoom();
	
    FVector2D StartRoomPosition(0, 0);
    FVector2D BossRoomPosition(5, 5);

    int32 Steps = 20;

    TArray<ASDungeonRoom*> MainPath;
    MainPath.Append(GenerateMainPath(StartingRooms[0], StartRoomPosition, BossRoomPosition, Steps, 3));
    */
}

/*
TArray<ASDungeonRoom*> ADungeonGenerator::GenerateMainPath(ASDungeonRoom* RootRoom, const FVector2D& StartPosition, const FVector2D& EndPosition, const int32& Steps, const int32& NumRoomsToKeep)
{
    return TArray<ASDungeonRoom*>();
}

#pragma region Init Functions

void ADungeonGenerator::InitializeGrid()
{
    RoomGrid.Reserve(GridSize * GridSize);

    for (int32 i = 0; i <= GridSize; i++)
    {
        for (int32 j = 0; j <= GridSize; j++)
        {
            FVector2D Cell = FVector2D(i, j);

            RoomGrid.Add(Cell, nullptr);
        }
    }
}

void ADungeonGenerator::PlaceStartRoom()
{
    if (StartingRoomClass == nullptr) return;

    FVector StartRoomLocation = FVector(0 * TileSize, 0 * TileSize, -100.0f);
    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(StartingRoomClass, StartRoomLocation, FRotator(0, -90.0f, 0));
    if (Room == nullptr) return;

    StartingRooms[0] = Room;
    AllRooms.Add(Room);

    RoomGrid[FVector2D(0, 0)] = Room;
}

void ADungeonGenerator::PlaceBossRoom()
{
    if (BossRoomClass == nullptr) return;

    FVector BossRoomLocation = FVector(5 * TileSize, 6 * TileSize, -600.0f);
    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(BossRoomClass, BossRoomLocation, FRotator::ZeroRotator);
    if (Room == nullptr) return;

    RoomGrid[FVector2D(5, 5)] = Room;
    RoomGrid[FVector2D(5, 6)] = Room;
    RoomGrid[FVector2D(6, 5)] = Room;
    RoomGrid[FVector2D(6, 6)] = Room;

    StartingRooms[1] = Room;
}

#pragma endregion
*/
