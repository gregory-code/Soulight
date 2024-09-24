// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SDungeonGenerationComponent.h"

#include "Engine/World.h"
#include "EngineUtils.h"

#include "Framework/SDungeonRoom.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

ASDungeonGenerationComponent::ASDungeonGenerationComponent()
{

}

void ASDungeonGenerationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
    // 3 is hard coded then +2 for start and boss room
    StartingRooms.SetNum(5);

    InitializeGrid();

    PlaceStartRoom();
    PlaceBossRoom();

    GenerateRooms(3);

    for (int32 i = 0; i < StartingRooms.Num(); i++)
    {
        ASDungeonRoom* Room = StartingRooms[i];
        if (Room)
        {
            // Log the index and the name of the room (or any relevant property)
            UE_LOG(LogTemp, Warning, TEXT("Room %d: %s"), i, *Room->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Room %d: nullptr"), i);
        }
    }

    for (int i = 0; i < StartingRooms.Num() - 1; i++)
    {
        GenerateHallways(i);
    }

    GenerateChests(MaxNumChests);
}

void ASDungeonGenerationComponent::InitializeGrid()
{
    Grid.SetNum(GridSize);
    
    Grid.Init(TArray<bool>(), GridSize);

    for (int32 i = 0; i < GridSize; i++)
    {
        Grid[i].Init(false, GridSize);
    }
}

void ASDungeonGenerationComponent::PlaceStartRoom()
{
    if (StartingRoomClass == nullptr) return;

    Grid[0][0] = true; // Mark start room in the grid

    FVector StartRoomLocation = FVector(0 * TileSize, 0 * TileSize, -100.0f);
    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(StartingRoomClass, StartRoomLocation, FRotator(0, -90.0f,0));
    if (Room) {
        StartingRooms[0] = Room;
    }

    UE_LOG(LogTemp, Warning, TEXT("Start Room spawned at (0, 0)"));
}

void ASDungeonGenerationComponent::PlaceBossRoom()
{
    if (BossRoomClass == nullptr) return;

    Grid[4][4] = true;
    Grid[4][5] = true;
    Grid[5][4] = true;
    Grid[5][5] = true;

    FVector BossRoomLocation = FVector(4 * TileSize, 4 * TileSize, -100.0f);
    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(BossRoomClass, BossRoomLocation, FRotator::ZeroRotator);
    StartingRooms[StartingRooms.Num() - 1] = Room;

    UE_LOG(LogTemp, Warning, TEXT("Boss Room placed from (4, 4) to (5, 5)"));
}

void ASDungeonGenerationComponent::GenerateRooms(const int32& NumRooms)
{
    for (int32 RoomCount = 0; RoomCount < NumRooms; RoomCount++)
    {
        bool bPlaced = false;
        while (!bPlaced)
        {
            int32 X = FMath::RandRange(0, GridSize - 1);
            int32 Y = FMath::RandRange(0, GridSize - 1);

            if (Grid[X][Y] == false)
            {
                FVector RoomLocation = FVector(X * TileSize, Y * TileSize, -100.0f);
                ASDungeonRoom* SpawnedRoom = GetWorld()->SpawnActor<ASDungeonRoom>(GetRandomRoom(), RoomLocation, FRotator::ZeroRotator);
                StartingRooms[RoomCount + 1] = SpawnedRoom;

                UE_LOG(LogTemp, Warning, TEXT("Cell Assigned: %d, %d"), X, Y);

                Grid[X][Y] = true;
                bPlaced = true;
            }
        }
    }
}

void ASDungeonGenerationComponent::GenerateHallways(const int32& Index)
{
    if (StartingRooms.Num() < 2) return;

    int32 CurrentRoomX = StartingRooms[Index]->GetActorLocation().X / TileSize;
    int32 CurrentRoomY = StartingRooms[Index]->GetActorLocation().Y / TileSize;

    if (Index + 1 >= StartingRooms.Num()) return;

    int32 TargetRoomX = StartingRooms[Index + 1]->GetActorLocation().X / TileSize;
    int32 TargetRoomY = StartingRooms[Index + 1]->GetActorLocation().Y / TileSize;

    int32 RoomDistanceX = FMath::Abs(CurrentRoomX - TargetRoomX);
    int32 RoomDistanceY = FMath::Abs(CurrentRoomY - TargetRoomY);

    int32 XDirection = (TargetRoomX - CurrentRoomX) >= 0 ? 1 : -1;
    int32 YDirection = (TargetRoomY - CurrentRoomY) >= 0 ? 1 : -1;

    if (RoomDistanceX == 0 && RoomDistanceY == 0) return;

    if (RoomDistanceX == 1 && RoomDistanceY == 1)
    {
        FVector CornerLocation = FVector(TargetRoomX * TileSize, CurrentRoomY * TileSize, -100.0f);

        if (!Grid[TargetRoomX][CurrentRoomY])
        {
            SpawnRoom(GetRandomRoom(), CornerLocation);
        }

        return;
    }

    if (RoomDistanceX > 0)
    {
        for (int32 i = 0; i < RoomDistanceX; i++)
        {
            FVector HallwayLocation = FVector((CurrentRoomX + i * XDirection) * TileSize, CurrentRoomY * TileSize, -100.0f);

            if (!Grid[CurrentRoomX + i * XDirection][CurrentRoomY])
            {
                SpawnHallways(HallwayLocation, FRotator(0.0f, 0.0f, 0.0f));
                Grid[CurrentRoomX + i * XDirection][CurrentRoomY] = true;
            }
        }

        if (RoomDistanceY > 0)
        {
            FVector CornerLocation = FVector(TargetRoomX * TileSize, CurrentRoomY * TileSize, -100.0f);
            if (!Grid[TargetRoomX][CurrentRoomY])
            {
                SpawnRoom(GetRandomRoom(), CornerLocation);
                Grid[TargetRoomX][CurrentRoomY] = true;
            }
        }
    }

    if (RoomDistanceY > 0)
    {
        for (int32 i = 0; i < RoomDistanceY; i++)
        {
            FVector HallwayLocation = FVector(TargetRoomX * TileSize, (CurrentRoomY + i * YDirection) * TileSize, -100.0f);

            if (!Grid[TargetRoomX][CurrentRoomY + i * YDirection])
            {
                SpawnHallways(HallwayLocation, FRotator(0.0f, 90.0f, 0.0f));
                Grid[TargetRoomX][CurrentRoomY + i * YDirection] = true;
            }
        }

        if (RoomDistanceX > 0)
        {
            FVector CornerLocation = FVector(TargetRoomX * TileSize, TargetRoomY * TileSize, -100.0f);
            if (!Grid[TargetRoomX][TargetRoomY])
            {
                SpawnRoom(GetRandomRoom(), CornerLocation);
                Grid[TargetRoomX][TargetRoomY] = true;
            }
        }
    }

    if (!Grid[TargetRoomX][TargetRoomY])
    {
        SpawnRoom(GetRandomRoom(), FVector(TargetRoomX * TileSize, TargetRoomY * TileSize, -100.0f));
        Grid[TargetRoomX][TargetRoomY] = true;
    }
}

void ASDungeonGenerationComponent::SpawnHallways(const FVector& Location, const FRotator& Rotation)
{
    if (HallwayClass)
    {
        int32 X = Location.X / TileSize;
        int32 Y = Location.Y / TileSize;

        if (X < 0 || Y < 0 || X >= GridSize || Y >= GridSize)
        {
            UE_LOG(LogTemp, Warning, TEXT("Out Of Range!"));
            return;
        }

        if (Grid[X][Y])
        {
            UE_LOG(LogTemp, Warning, TEXT("Cell Taken: %d, %d"), X, Y);
            return;
        }

        ASDungeonRoom* Hallway = GetWorld()->SpawnActor<ASDungeonRoom>(HallwayClass, Location, Rotation);
        if (Hallway)
        {
            Grid[X][Y] = true;
        }
    }
}

void ASDungeonGenerationComponent::SpawnRoom(TSubclassOf<ASDungeonRoom> RoomClass, FVector Location)
{
    if (RoomClass)
    {
        int32 X = Location.X / TileSize;
        int32 Y = Location.Y / TileSize;

        if (X < 0 || Y < 0 || X >= GridSize || Y >= GridSize)
        {
            UE_LOG(LogTemp, Warning, TEXT("Out Of Range!"));
            return;
        }

        // Check if grid cell is already taken
        if (Grid[X][Y])
        {
            UE_LOG(LogTemp, Warning, TEXT("Cell Taken: %d, %d"), X, Y);
            return;
        }

        // Mark grid as occupied before spawning
        Grid[X][Y] = true;

        // Now spawn the room
        ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(RoomClass, Location, FRotator::ZeroRotator);
        if (Room)
        {
            if (Room->GetChestSpawnPoints().Num() > 0)
            {
                ChestSpawnPoints.Append(Room->GetChestSpawnPoints());
            }
        }
    }
}

void ASDungeonGenerationComponent::GenerateChests(const int32& NumChests)
{
    if (ChestClass == nullptr) return;

    if (NumChests == 0 || ChestSpawnPoints.Num() == 0) return;

    for (int i = NumChests; i > 0; i--)
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

TSubclassOf<ASDungeonRoom> ASDungeonGenerationComponent::GetRandomRoom()
{
    if (RoomClasses.Num() == 0) return nullptr;

    int32 RandomNumber = FMath::RandRange(0, RoomClasses.Num() - 1);

    return RoomClasses[RandomNumber];
}
