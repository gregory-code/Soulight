// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SDungeonGenerationComponent.h"

#include "Engine/World.h"
#include "EngineUtils.h"

#include "Framework/SDungeonRoom.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

USDungeonGenerationComponent::USDungeonGenerationComponent()
{

}

void USDungeonGenerationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
    // 3 is hard coded then +2 for start and boss room
    InstancedRooms.SetNum(5);

    InitializeGrid();
    PlaceStartRoom();
    PlaceBossRoom();

    GenerateRooms(3);

    for (int32 i = 0; i < InstancedRooms.Num(); i++)
    {
        ASDungeonRoom* Room = InstancedRooms[i];
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

    for (int i = 0; i < InstancedRooms.Num() - 1; i++)
    {
        GenerateHallways(i);
    }

    GenerateChests(MaxNumChests);
}

void USDungeonGenerationComponent::InitializeGrid()
{
    Grid.SetNum(GridSize);
    
    Grid.Init(TArray<bool>(), GridSize);

    for (int32 i = 0; i < GridSize; i++)
    {
        Grid[i].Init(false, GridSize);
    }
}

void USDungeonGenerationComponent::PlaceStartRoom()
{
    if (StartingRoomClass == nullptr) return;

    Grid[0][0] = true; // Mark start room in the grid

    FVector StartRoomLocation = FVector(0 * TileSize, 0 * TileSize, -100.0f);
    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(StartingRoomClass, StartRoomLocation, FRotator::ZeroRotator);
    if (Room) {
        InstancedRooms[0] = Room;
    }

    UE_LOG(LogTemp, Warning, TEXT("Start Room spawned at (0, 0)"));
}

void USDungeonGenerationComponent::PlaceBossRoom()
{
    if (BossRoomClass == nullptr) return;

    Grid[4][4] = true; // Mark boss room in the grid
    Grid[4][5] = true;
    Grid[5][4] = true;
    Grid[5][5] = true;

    // Boss room location at (4,4) (bottom-left of the 2x2 room)
    FVector BossRoomLocation = FVector(4 * TileSize, 4 * TileSize, -100.0f);
    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(BossRoomClass, BossRoomLocation, FRotator::ZeroRotator);
    InstancedRooms[InstancedRooms.Num() - 1] = Room;

    UE_LOG(LogTemp, Warning, TEXT("Boss Room placed from (4, 4) to (5, 5)"));
}

void USDungeonGenerationComponent::GenerateRooms(const int32& NumRooms)
{
    for (int32 RoomCount = 0; RoomCount < NumRooms; RoomCount++)
    {
        bool bPlaced = false;
        while (!bPlaced)
        {
            int32 X = FMath::RandRange(0, GridSize - 1);
            int32 Y = FMath::RandRange(0, GridSize - 1);

            if (!Grid[X][Y]) // Check if the cell is empty
            {
                FVector RoomLocation = FVector(X * TileSize, Y * TileSize, -100.0f);
                ASDungeonRoom* SpawnedRoom = GetWorld()->SpawnActor<ASDungeonRoom>(StartingRoomClass, RoomLocation, FRotator::ZeroRotator);
                InstancedRooms[RoomCount + 1] = SpawnedRoom;

                UE_LOG(LogTemp, Warning, TEXT("Cell Assigned: %d, %d"), X, Y);

                Grid[X][Y] = true; // Mark the cell as occupied
                bPlaced = true;
            }
        }
    }
}

void USDungeonGenerationComponent::GenerateHallways(const int32& Index)
{
    if (InstancedRooms.Num() < 2) return;

    int32 CurrentRoomX = InstancedRooms[Index]->GetActorLocation().X / TileSize;
    int32 CurrentRoomY = InstancedRooms[Index]->GetActorLocation().Y / TileSize;

    if (Index + 1 >= InstancedRooms.Num()) return;

    int32 TargetRoomX = InstancedRooms[Index + 1]->GetActorLocation().X / TileSize;
    int32 TargetRoomY = InstancedRooms[Index + 1]->GetActorLocation().Y / TileSize;

    int32 RoomDistanceX = FMath::Abs(CurrentRoomX - TargetRoomX);
    int32 RoomDistanceY = FMath::Abs(CurrentRoomY - TargetRoomY);

    int32 XDirection = (TargetRoomX - CurrentRoomX) >= 0 ? 1 : -1;
    int32 YDirection = (TargetRoomY - CurrentRoomY) >= 0 ? 1 : -1;

    // Check for room placement
    if (RoomDistanceX == 0 && RoomDistanceY == 0) return;

    // Handle corners as rooms
    if (RoomDistanceX == 1 && RoomDistanceY == 1)
    {
        // Spawn a corner room
        FVector CornerLocation = FVector(TargetRoomX * TileSize, TargetRoomY * TileSize, -100.0f);
        SpawnRoom(StartingRoomClass, CornerLocation);
        return;
    }

    // Generate Hallways
    if (RoomDistanceX > 0) // Horizontal Movement
    {
        for (int32 i = 0; i < RoomDistanceX; i++)
        {
            FVector HallwayLocation = FVector((CurrentRoomX + i * XDirection) * TileSize, CurrentRoomY * TileSize, -100.0f);
            SpawnHallways(HallwayLocation, FRotator(0.0f, 0.0f, 0.0f));
        }

        if (RoomDistanceY > 0)
        {
            FVector CornerLocation = FVector(TargetRoomX * TileSize, CurrentRoomY * TileSize, -100.0f);
            SpawnRoom(StartingRoomClass, CornerLocation); // Corner room for vertical connection
        }
    }

    if (RoomDistanceY > 0) // Vertical Movement
    {
        for (int32 i = 0; i < RoomDistanceY; i++)
        {
            FVector HallwayLocation = FVector(TargetRoomX * TileSize, (CurrentRoomY + i * YDirection) * TileSize, -100.0f);
            SpawnHallways(HallwayLocation, FRotator(0.0f, 90.0f, 0.0f));
        }

        if (RoomDistanceX > 0)
        {
            FVector CornerLocation = FVector(TargetRoomX * TileSize, TargetRoomY * TileSize, -100.0f);
            SpawnRoom(StartingRoomClass, CornerLocation); // Corner room for horizontal connection
        }
    }

    // Ensure to connect to the next room
    SpawnRoom(StartingRoomClass, FVector(TargetRoomX * TileSize, TargetRoomY * TileSize, -100.0f));
}

void USDungeonGenerationComponent::SpawnHallways(const FVector& Location, const FRotator& Rotation)
{
    if (HallwayClass)
    {
        int32 X = Location.X / TileSize;
        int32 Y = Location.Y / TileSize;

        // No need to adjust X or Y here
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
            Grid[X][Y] = true; // Mark as occupied after spawning
        }
    }
}

void USDungeonGenerationComponent::SpawnRoom(TSubclassOf<ASDungeonRoom> RoomClass, FVector Location)
{
    if (RoomClass)
    {
        int32 X = Location.X / TileSize;
        int32 Y = Location.Y / TileSize;

        // No need to adjust X or Y here
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

        ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(RoomClass, Location, FRotator::ZeroRotator);
        if (Room)
        {
            Grid[X][Y] = true; // Mark as occupied after spawning

            if (Room->GetChestSpawnPoints().Num() > 0)
                ChestSpawnPoints.Append(Room->GetChestSpawnPoints());
        }
    }
}

void USDungeonGenerationComponent::GenerateChests(const int32& NumChests)
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

#pragma region Original Code

/*

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

    return Directions;
}

void USDungeonGenerationComponent::RotateRoomBasedOnDirection(ERoomOpeningDirection Direction, ASDungeonRoom* Room)
{

}

#pragma endregion

*/

#pragma endregion 