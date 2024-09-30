// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SDungeonGenerationComponent.h"

#include "Engine/World.h"
#include "EngineUtils.h"

#include "Framework/SDungeonRoom.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// IDEAS
// I could make the generation mark grid cells for rooms, then make hallways connecting them, after I then place the proper rooms

ASDungeonGenerationComponent::ASDungeonGenerationComponent()
{

}

#pragma region Initialize Functions

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

    // Add Boss Room As Last Room in Generation
    AllRooms.Add(StartingRooms[StartingRooms.Num() - 1]);

    // Add Start Room Before Hallway Generation
    
    // Start Hallway Generation
    for (int i = 0; i < StartingRooms.Num() - 1; i++)
    {
        GenerateHallways(i);
    }

    
    GenerateChests(MaxNumChests);

    for (int i = 1; i < AllRooms.Num() - 1; i++) 
    {
        FindBestRoomTilePiece(AllRooms[i]);
    }

    /*
    for (int i = 0; i < AllRooms.Num() - 1; i++)
    {
        AddRoomNeighbors(AllRooms[i]);
    }
    */
}

void ASDungeonGenerationComponent::InitializeGrid()
{
    RoomGrid.Reserve(GridSize * GridSize);

    for (int32 i = 0; i < GridSize; i++)
    {
        for (int32 j = 0; j < GridSize; j++)
        {
            FVector2D Cell = FVector2D(i, j);

            RoomGrid.Add(Cell, nullptr);
        }
    }
}

void ASDungeonGenerationComponent::PlaceStartRoom()
{
    if (StartingRoomClass == nullptr) return;
    
    FVector StartRoomLocation = FVector(0 * TileSize, 0 * TileSize, -100.0f);
    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(StartingRoomClass, StartRoomLocation, FRotator(0, -90.0f,0));
    if (Room == nullptr) return; 
    
    StartingRooms[0] = Room;
    AllRooms.Add(Room);

    RoomGrid[FVector2D(0, 0)] = Room;

    UE_LOG(LogTemp, Warning, TEXT("Start Room spawned at (0, 0)"));
}

void ASDungeonGenerationComponent::PlaceBossRoom()
{
    if (BossRoomClass == nullptr) return;

    FVector BossRoomLocation = FVector(4 * TileSize, 4 * TileSize, -600.0f);
    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(BossRoomClass, BossRoomLocation, FRotator::ZeroRotator);
    if (Room == nullptr) return;

    RoomGrid[FVector2D(4, 4)] = Room;
    RoomGrid[FVector2D(4, 5)] = Room;
    RoomGrid[FVector2D(5, 4)] = Room;
    RoomGrid[FVector2D(5, 5)] = Room;

    StartingRooms[StartingRooms.Num() - 1] = Room;

    UE_LOG(LogTemp, Warning, TEXT("Boss Room placed from (4, 4) to (5, 5)"));
}

#pragma endregion

#pragma region Room Generation Functions

void ASDungeonGenerationComponent::GenerateRooms(const int32& NumRooms)
{
    for (int32 RoomCount = 0; RoomCount < NumRooms; RoomCount++)
    {
        bool bPlaced = false;
        while (!bPlaced)
        {
            int32 X = FMath::RandRange(0, GridSize - 3);
            int32 Y = FMath::RandRange(0, GridSize - 3);

            if (RoomGrid[FVector2D(X, Y)] == nullptr)
            {
                FVector RoomLocation = FVector(X * TileSize, Y * TileSize, -100.0f);
                ASDungeonRoom* SpawnedRoom = GetWorld()->SpawnActor<ASDungeonRoom>(GetRandomRoom(), RoomLocation, FRotator::ZeroRotator);
                if (SpawnedRoom == nullptr) continue;

                UE_LOG(LogTemp, Warning, TEXT("Cell Assigned: %d, %d"), X, Y);

                StartingRooms[RoomCount + 1] = SpawnedRoom;
                AllRooms.Add(SpawnedRoom);

                RoomGrid[FVector2D(X, Y)] = SpawnedRoom;
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

        if (RoomGrid[FVector2D(TargetRoomX, CurrentRoomY)] == nullptr)
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

            if (RoomGrid[FVector2D(CurrentRoomX + i * XDirection, CurrentRoomY)] == nullptr)
            {
                //UE_LOG(LogTemp, Warning, TEXT("Room Index: %d, Hallway Index: %d"), Index, i);

                SpawnHallway(Index, i, HallwayLocation, FRotator(0.0f, 0.0f, 0.0f));
            }
        }

        if (RoomDistanceY > 0)
        {
            FVector CornerLocation = FVector(TargetRoomX * TileSize, CurrentRoomY * TileSize, -100.0f);
            if (RoomGrid[FVector2D(TargetRoomX, CurrentRoomY)] == nullptr)
            {
                SpawnRoom(GetRandomRoom(), CornerLocation);
            }
        }
    }

    if (RoomDistanceY > 0)
    {
        for (int32 i = 0; i < RoomDistanceY; i++)
        {
            FVector HallwayLocation = FVector(TargetRoomX * TileSize, (CurrentRoomY + (i * YDirection)) * TileSize, -100.0f);

            if (RoomGrid[FVector2D(TargetRoomX, CurrentRoomY + (i * YDirection))] == nullptr)
            {
                //UE_LOG(LogTemp, Warning, TEXT("Room Index: %d, Hallway Index: %d"), Index, i);

                SpawnHallway(Index, i, HallwayLocation, FRotator(0.0f, 90.0f, 0.0f));
            }
        }

        if (RoomDistanceX > 0)
        {
            FVector CornerLocation = FVector(TargetRoomX * TileSize, TargetRoomY * TileSize, -100.0f);
            if (RoomGrid[FVector2D(TargetRoomX, TargetRoomY)] == nullptr)
            {
                SpawnRoom(GetRandomRoom(), CornerLocation);
            }
        }
    }

    if (RoomGrid[FVector2D(TargetRoomX, TargetRoomY)] == nullptr)
    {
        SpawnRoom(GetRandomRoom(), FVector(TargetRoomX * TileSize, TargetRoomY * TileSize, -100.0f));
    }
}

void ASDungeonGenerationComponent::SpawnHallway(const int32& RoomIndex, const int32& HallwayIndex, const FVector& Location, const FRotator& Rotation)
{
    if (HallwayClass == nullptr) return;

    int32 X = Location.X / TileSize;
    int32 Y = Location.Y / TileSize;

    //X = X > 0 ? X - 1 : 0;
    //Y = Y > 0 ? Y - 1 : 0;

    UE_LOG(LogTemp, Warning, TEXT("Hallway Cell: %d, %d"), X, Y);

    if (X < 0 || Y < 0 || X >= GridSize || Y >= GridSize)
    {
        UE_LOG(LogTemp, Warning, TEXT("Out Of Range!"));
        return;
    }

    if (RoomGrid[FVector2D(X, Y)] == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cell Taken"));
        return;
    }

    ASDungeonRoom* Hallway = GetWorld()->SpawnActor<ASDungeonRoom>(HallwayClass, Location, Rotation);
    if (Hallway == nullptr) return;

    // If (HallwayList.Num() == 0) I am the start of the list, add me to it
    // Also add hallway to the starting room neighbor

    HallwayList.Add(Hallway);

    UE_LOG(LogTemp, Warning, TEXT("Hallway Index: %d, List Num: %d"), HallwayIndex - 1, HallwayList.Num());

    // also check if list is not 0
    if (HallwayIndex > 0 && HallwayIndex < HallwayList.Num())
    {
        Hallway->AddChildRoom(HallwayList[HallwayIndex - 1]);
        HallwayList[HallwayIndex - 1]->AddChildRoom(Hallway);
    }
    else if (HallwayIndex == 0) // Hallway Index <= 0
    {
        int32 Index = RoomIndex <= 0 ? 0 : RoomIndex;

        Hallway->AddChildRoom(AllRooms[Index]);
        AllRooms[Index]->AddChildRoom(Hallway);
    }

    // If end piece, Add self to room neighbors

    RoomGrid[FVector2D(X, Y)] = Hallway;
}

ASDungeonRoom* ASDungeonGenerationComponent::SpawnRoom(TSubclassOf<ASDungeonRoom> RoomClass, FVector Location)
{
    if (RoomClass == nullptr) return nullptr;

    int32 X = Location.X / TileSize;
    int32 Y = Location.Y / TileSize;

    if (X < 0 || Y < 0 || X >= GridSize || Y >= GridSize)
    {
        UE_LOG(LogTemp, Warning, TEXT("Out Of Range!"));
        return nullptr;
    }

    if (RoomGrid[FVector2D(X, Y)])
    {
        UE_LOG(LogTemp, Warning, TEXT("Cell Taken: %d, %d"), X, Y);
        return nullptr;
    }

    if (X >= 4 && Y >= 4)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cell Is Overlapping Boss Room"));
        return nullptr;
    }


    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(RoomClass, Location, FRotator::ZeroRotator);
    if (Room == nullptr) return nullptr;

    RoomGrid[FVector2D(X, Y)] = Room;

    // Add Neighbors
    if (AllRooms.Num() > 0)
    {
        ASDungeonRoom* PreviousRoom = AllRooms[AllRooms.Num() - 1];
        if (PreviousRoom) {
            PreviousRoom->AddChildRoom(Room);
            Room->AddChildRoom(PreviousRoom);
        }
    }

    // Then Add Room to List
    AllRooms.Add(Room);

    if (Room->GetChestSpawnPoints().Num() > 0)
    {
        ChestSpawnPoints.Append(Room->GetChestSpawnPoints());
    }

    return Room;
}

ASDungeonRoom* ASDungeonGenerationComponent::GenerateNewTilePiece(ASDungeonRoom* TargetRoom, const int32& RoomNeighbors)
{
    if (RoomMap[RoomNeighbors] == nullptr || TargetRoom == nullptr) return nullptr;

    FVector RoomLocation = TargetRoom->GetActorLocation();

    ASDungeonRoom* NewRoom;
    if (IsCornerRoom(TargetRoom) && CornerRoomClass != nullptr)
        NewRoom = GetWorld()->SpawnActor<ASDungeonRoom>(CornerRoomClass, RoomLocation, FRotator::ZeroRotator);
    else
        NewRoom = GetWorld()->SpawnActor<ASDungeonRoom>(RoomMap[RoomNeighbors], RoomLocation, FRotator::ZeroRotator);

    if (NewRoom == nullptr) return nullptr;

    int32 Index = AllRooms.IndexOfByKey(TargetRoom);
    if (Index != INDEX_NONE)
        AllRooms[Index] = NewRoom;

    TargetRoom->Destroy();

    return NewRoom;
}

#pragma endregion

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

void ASDungeonGenerationComponent::AddRoomNeighbors(ASDungeonRoom* TargetRoom)
{
    // I am here today

    const FVector2D* RoomLocation = RoomGrid.FindKey(TargetRoom);

    if (RoomLocation == nullptr) return;

    if (RoomGrid.Contains(FVector2D(RoomLocation->X - 1, RoomLocation->Y)))
        TargetRoom->AddChildRoom(RoomGrid[FVector2D(RoomLocation->X - 1, RoomLocation->Y)]);

    if (RoomGrid.Contains(FVector2D(RoomLocation->X + 1, RoomLocation->Y)))
        TargetRoom->AddChildRoom(RoomGrid[FVector2D(RoomLocation->X - 1, RoomLocation->Y)]);

    if (RoomGrid.Contains(FVector2D(RoomLocation->X, RoomLocation->Y - 1)))
        TargetRoom->AddChildRoom(RoomGrid[FVector2D(RoomLocation->X, RoomLocation->Y - 1)]);

    if (RoomGrid.Contains(FVector2D(RoomLocation->X, RoomLocation->Y + 1)))
        TargetRoom->AddChildRoom(RoomGrid[FVector2D(RoomLocation->X, RoomLocation->Y + 1)]);
}

void ASDungeonGenerationComponent::FindBestRoomTilePiece(ASDungeonRoom* TargetRoom)
{
    if (TargetRoom == nullptr) return;

    int32 RoomOpenings = TargetRoom->Openings.Num();
    int32 RoomNeighbors = TargetRoom->GetChildrenRoom().Num();

    if (RoomOpenings < RoomNeighbors) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Not Enough Opening For Neighbors, Generate New Tile Piece!"));

        // TODO: v This Right Here v
        GenerateNewTilePiece(TargetRoom, RoomNeighbors);
    }

    if (IsCornerRoom(TargetRoom)) 
    {
        UE_LOG(LogTemp, Warning, TEXT("I AM A CORNER!"));

        GenerateNewTilePiece(TargetRoom, RoomNeighbors);
    }

    UE_LOG(LogTemp, Warning, TEXT("Finding Rotation!"));

    FQuat TargetRot = FQuat::MakeFromRotator(CalculateRoomRotation(TargetRoom));
    TargetRoom->SetActorRotation(TargetRot);
}

#pragma region Helper Functions

bool ASDungeonGenerationComponent::IsCornerRoom(ASDungeonRoom* TargetRoom)
{
    if (TargetRoom->GetChildrenRoom().Num() != 2) return false;
   
    float X1 = TargetRoom->GetChildrenRoom()[0]->GetActorLocation().X;
    float X2 = TargetRoom->GetChildrenRoom()[1]->GetActorLocation().X;

    float Y1 = TargetRoom->GetChildrenRoom()[0]->GetActorLocation().Y;
    float Y2 = TargetRoom->GetChildrenRoom()[1]->GetActorLocation().Y;

    // If both neighbors aren't on the same X or Y level then we know the neighbors
    // must be adjacent to one another not parallel.
    if (FMath::IsNearlyEqual(X1, X2, 0.01f) == false && FMath::IsNearlyEqual(Y1, Y2, 0.01f) == false)
    {
        return true;
    }

    return false;
}

FRotator ASDungeonGenerationComponent::CalculateRoomRotation(ASDungeonRoom* TargetRoom)
{
    FRotator Target = FRotator::ZeroRotator;

    if (TargetRoom->GetChildrenRoom().Num() == 0 || TargetRoom->Openings.Num() == 0)
        return Target;

    if (TargetRoom->Openings.Num() == 4)
        return Target;

    // Adjust rotation based on neighbors
    int32 UpNeighbors = 0;
    int32 DownNeighbors = 0;
    int32 LeftNeighbors = 0;
    int32 RightNeighbors = 0;

    for (ASDungeonRoom* Neighbor : TargetRoom->GetChildrenRoom())
    {
        if (Neighbor)
        {
            FVector NeighborPos = Neighbor->GetActorLocation();

            if (NeighborPos.X > TargetRoom->GetActorLocation().X)
            {
                RightNeighbors++;
            }
            else if (NeighborPos.X < TargetRoom->GetActorLocation().X)
            {
                LeftNeighbors++;
            }
            else if (NeighborPos.Y > TargetRoom->GetActorLocation().Y)
            {
                UpNeighbors++;
            }
            else if (NeighborPos.Y < TargetRoom->GetActorLocation().Y)
            {
                DownNeighbors++;
            }
        }
    }

    // I only have vertical neighbors
    // I am also assuming that the previous pass makes sure this room is not a corner
    if (UpNeighbors > 0 && DownNeighbors > 0 && LeftNeighbors == 0 && RightNeighbors == 0) 
    {
        // All rooms by default should have initial rotation with the start opening being at the bottom

        Target.Yaw = 0.0f;

        return Target;
    }

    // I only have horizontal neighbors
    if (UpNeighbors == 0 && DownNeighbors == 0 && LeftNeighbors > 0 && RightNeighbors > 0)
    {
        Target.Yaw = 90.0f;

        return Target;
    }

    // I am a corner

    int32 VertNeighbors = UpNeighbors - DownNeighbors;
    int32 HoriNeighbors = LeftNeighbors - RightNeighbors;
    if (VertNeighbors != 0 && HoriNeighbors != 0)
    {
        if (VertNeighbors > 0 && HoriNeighbors > 0)
            Target.Yaw = 180.0f;
        if(VertNeighbors > 0 && HoriNeighbors < 0)
            Target.Yaw = -90.0f;
        if (VertNeighbors < 0 && HoriNeighbors < 0)
            Target.Yaw = 0.0f;
        if (VertNeighbors < 0 && HoriNeighbors > 0)
            Target.Yaw = 90.0f;

        return Target;
    }

    return Target;


    // I have 3 neighbors, 2 on the side one vertical

    // I have 3 neighbors, 2 vertical one on the side

    // Determine the dominant direction based on neighbors
    if (UpNeighbors >= DownNeighbors && UpNeighbors >= LeftNeighbors && UpNeighbors >= RightNeighbors)
    {
        Target.Yaw = 0.0f;
    }
    else if (DownNeighbors >= UpNeighbors && DownNeighbors >= LeftNeighbors && DownNeighbors >= RightNeighbors)
    {
        Target.Yaw = 180.0f;
    }
    else if (LeftNeighbors >= UpNeighbors && LeftNeighbors >= DownNeighbors && LeftNeighbors >= RightNeighbors)
    {
        Target.Yaw = -90.0f;
    }
    else if (RightNeighbors >= UpNeighbors && RightNeighbors >= DownNeighbors && RightNeighbors >= LeftNeighbors)
    {
        Target.Yaw = 90.0f;
    }

    return Target;
}

TSubclassOf<ASDungeonRoom> ASDungeonGenerationComponent::GetRandomRoom()
{
    if (RoomClasses.Num() == 0) return nullptr;

    int32 RandomNumber = FMath::RandRange(0, RoomClasses.Num() - 1);

    return RoomClasses[RandomNumber];
}

#pragma endregion
