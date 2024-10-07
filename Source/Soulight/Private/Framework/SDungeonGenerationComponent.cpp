// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SDungeonGenerationComponent.h"

#include "Engine/World.h"
#include "EngineUtils.h"

#include "Framework/SDungeonRoom.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

    FVector2D StartRoomPosition(0, 0);
    FVector2D BossRoomPosition(5, 5);

    int32 Steps = 15;

    TArray<ASDungeonRoom*> IntermediatePath;
    IntermediatePath.Append(WalkTowardsEnd(StartRoomPosition, BossRoomPosition, Steps, 2));

    GenerateBranches(IntermediatePath);

    FRotator TargetRotation(0, -90.0f, 0);

    FQuat TargetQuat;
    TargetQuat = FQuat::MakeFromRotator(TargetRotation);
    StartingRooms[0]->SetActorRotation(TargetQuat);

    TargetRotation = FRotator(0, 0, 0);
    TargetQuat = FQuat::MakeFromRotator(TargetRotation);
    StartingRooms[1]->SetActorRotation(TargetQuat);

    GenerateDeadEnds();

    return;

    GenerateChests(MaxNumChests);
}

//

#pragma region Initial Setup Walking Functions
//

TArray<ASDungeonRoom*> ASDungeonGenerationComponent::WalkTowardsEnd(const FVector2D StartPosition, const FVector2D EndPosition, const int32& Steps, const int32& NumRoomsToKeep)
{
    TArray<ASDungeonRoom*> GeneratedRooms;

    GeneratedRooms.Append(WalkingGeneration(Steps, StartPosition, EndPosition));

    ReplaceRoomsWithHallways(GeneratedRooms, NumRoomsToKeep);

    //GeneratedRooms.Insert(StartingRooms[0], 0);
    //GeneratedRooms.Add(StartingRooms[1]);

    // TODO: Logic to handle corners, T Rooms, and Intersections. Then Start Generating Dead Ends and Branches.
    // ALSO TODO: Figure out why the ReplaceRoomsWithHallways() function isn't working as intended.\

    CheckForCorners(GeneratedRooms);

    for (ASDungeonRoom* Room : GeneratedRooms)
    {
        if (Room == nullptr) continue;

        FRotator TargetRotation = CalculateRoomRotation(Room);

        FQuat TargetQuat;
        TargetQuat = FQuat::MakeFromRotator(TargetRotation);
        Room->SetActorRotation(TargetQuat);
    }

    // Hit it with that double check to make damn sure it's oriented properly

    if (GeneratedRooms.Num() > 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("PLEASE WORK I BEG OF YOU"));

        if (IsValid(GeneratedRooms[GeneratedRooms.Num() - 1]))
        {
            UE_LOG(LogTemp, Warning, TEXT("PLEASE WORK I BEG OF YOU, PART 2"));

            FRotator TargetRotation = CalaculateRotationFromRoomPosition(GeneratedRooms[GeneratedRooms.Num() - 2], GeneratedRooms[GeneratedRooms.Num() - 1]);
            
            FQuat TargetQuat;
            TargetQuat = FQuat::MakeFromRotator(TargetRotation);
            GeneratedRooms[GeneratedRooms.Num() - 1]->SetActorRotation(TargetQuat);
        }

        if(GeneratedRooms.Last() != nullptr)
            UE_LOG(LogTemp, Warning, TEXT("PROBLEM CHILD: %s"), *GeneratedRooms.Last()->GetName());
    }


    return GeneratedRooms;
}

FVector2D ASDungeonGenerationComponent::GetRandomMoveDirection(const FVector2D& CurrentPos, const FVector2D& EndRoomPosition, float& OutRotation)
{
    /*

    // Randomly decide to move right or up, favoring towards the boss room
    TArray<FVector2D> NeighborCells;
    NeighborCells.Append(GetPossibleNeighborCells(CurrentPos));

    if (NeighborCells.Num() <= 0)
    {
        OutRotation = 0.0f;
        return FVector2D(0, 0);
    }
    */

    // Get Random Direction IF I don't have that anything in that direction

    if (CurrentPos.X < EndRoomPosition.X && FMath::RandBool())
    {
        OutRotation = 0.0f;
        return FVector2D(1, 0); // Move right
    }
    else if (CurrentPos.Y < EndRoomPosition.Y)
    {
        OutRotation = 90.0f;
        return FVector2D(0, 1); // Move up
    }
    else if (CurrentPos.Y < EndRoomPosition.Y && FMath::RandBool())
    {
        OutRotation = 90.0f;
        return FVector2D(0, 1); // Move up if not moving right
    }
    else if (CurrentPos.X < EndRoomPosition.X)
    {
        OutRotation = 0.0f;
        return FVector2D(1, 0); // Move right if not moving up
    }

    OutRotation = 90.0f;
    return FVector2D(0, 0);
}

TArray<ASDungeonRoom*> ASDungeonGenerationComponent::WalkingGeneration(const int32& Steps, const FVector2D& StartingPosition, const FVector2D& EndRoomPosition)
{
    TArray<ASDungeonRoom*> GeneratedRooms;

    FVector2D CurrentPosition = StartingPosition;
    int32 StepsTaken = 0;

    // Randomly walk towards the end
    while (StepsTaken < Steps)
    {
        FVector2D MoveDirection(0, 0);
        float Rotation = 90.0f;

        bool bMoved = false;

        MoveDirection = GetRandomMoveDirection(CurrentPosition, EndRoomPosition, Rotation);

        FVector2D NextPosition = CurrentPosition + MoveDirection;

        // Ensure the next position is not the boss room
        if (NextPosition != EndRoomPosition && CanWalkInDirection(NextPosition))
        {
            // Check if the next position exists in the RoomGrid
            if (RoomGrid.Contains(NextPosition))
            {
                ASDungeonRoom* CurrentRoom = RoomGrid[NextPosition];

                // If the room doesn't exist, attempt to spawn a new room
                if (CurrentRoom == nullptr)
                {
                    FVector SpawnLocation = FVector(NextPosition.X * TileSize, NextPosition.Y * TileSize, -100.0f);
                    CurrentRoom = SpawnRoom(RoomMap[4], SpawnLocation, FRotator(0, Rotation, 0));

                    RoomGrid[NextPosition] = CurrentRoom; // Store the new room in the grid
                }

                /* Idea I had to make this do the rotation as it generated, didn't work to well for corners
                if (StepsTaken > 0 && StepsTaken < GeneratedRooms.Num())
                {
                    FRotator TargetRotation = CalaculateRotationFromRoomPosition(GeneratedRooms[StepsTaken], CurrentRoom);

                    FQuat TargetQuat;
                    TargetQuat = FQuat::MakeFromRotator(TargetRotation);
                    CurrentRoom->SetActorRotation(TargetQuat);

                }
                */

                if (CurrentRoom != nullptr)
                    UE_LOG(LogTemp, Warning, TEXT("I AM GENERATING ROOM: %s"), *CurrentRoom->GetName());

                GeneratedRooms.Add(CurrentRoom);

                // Move to the existing room
                CurrentPosition = NextPosition;
                StepsTaken++;
                bMoved = true;
            }
        }

        // If no movement was made in this iteration, exit the loop to prevent infinite looping
        if (!bMoved)
        {
            UE_LOG(LogTemp, Warning, TEXT("No valid moves available, breaking out of loop."));
            break; // Exit the loop if no valid movement was made
        }
    }

    return GeneratedRooms;
}

bool ASDungeonGenerationComponent::CanWalkInDirection(const FVector2D& Direction)
{
    if (RoomGrid[Direction] == nullptr)
        return true;

    return false;
}

void ASDungeonGenerationComponent::ReplaceRoomsWithHallways(TArray<ASDungeonRoom*>& Rooms, const int32& NumRoomsToKeep)
{
    if (Rooms.Num() <= NumRoomsToKeep) return;

    // Calculate path length
    int32 PathLength = Rooms.Num();

    // Calculate the interval to evenly spread the rooms
    float Interval = PathLength / static_cast<float>(NumRoomsToKeep);

    // Ensure a set of rooms to keep at even intervals
    TSet<int32> RoomsToKeep;
    for (int32 i = 0; i < NumRoomsToKeep; i++)
    {
        // Calculate the index to keep based on the interval
        int32 RoomIndex = FMath::RoundToInt(i * Interval);

        // Ensure the room index is within bounds
        if (RoomIndex >= 0 && RoomIndex < Rooms.Num())
        {
            RoomsToKeep.Add(RoomIndex);
        }
    }

    // Replace rooms that are not in the RoomsToKeep set with hallways
    for (int32 i = 0; i < Rooms.Num(); i++)
    {
        if (!RoomsToKeep.Contains(i) && IsValid(Rooms[i]))
        {
            // Spawn a new hallway (assuming you have a method to do this)
            ASDungeonRoom* NewHallway = GetWorld()->SpawnActor<ASDungeonRoom>(HallwayClass, Rooms[i]->GetActorLocation(), Rooms[i]->GetActorRotation());

            if (NewHallway != nullptr) {
                UE_LOG(LogTemp, Warning, TEXT("I AM GENERATING Hallway: %s"), *NewHallway->GetName());
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("I AM NULL WHY THE FUCK AM I NULL"));
            }

            const FVector2D* Cell = RoomGrid.FindKey(Rooms[i]);
            if (Cell == nullptr) return;

            // Destroy the current room
            Rooms[i]->Destroy();

            // Replace the current element with the new hallway
            Rooms[i] = NewHallway;
            RoomGrid[*Cell] = NewHallway;
        }
    }
}

void ASDungeonGenerationComponent::CheckForCorners(TArray<ASDungeonRoom*>& Rooms)
{
    if (Rooms.Num() <= 0 || CornerHallwayClass == nullptr) return;

    for (int32 i = 0; i < Rooms.Num(); i++) 
    {
        if (Rooms[i] == nullptr) continue;

        if (Rooms[i]->GetIsHallway() == false) continue;

        if (IsCornerRoom(Rooms[i]))
        {
            UE_LOG(LogTemp, Warning, TEXT("Room at index %d is a corner!"), i);

            ASDungeonRoom* HallwayCorner = GetWorld()->SpawnActor<ASDungeonRoom>(CornerHallwayClass, Rooms[i]->GetActorLocation(), Rooms[i]->GetActorRotation());
            const FVector2D* Cell = RoomGrid.FindKey(Rooms[i]);

            if (Cell == nullptr)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to find cell in RoomGrid!"));
                continue;
            }

            Rooms[i]->Destroy();
            Rooms[i] = HallwayCorner;
            RoomGrid[*Cell] = HallwayCorner;  // Ensure this updates RoomGrid properly

            UE_LOG(LogTemp, Warning, TEXT("Replaced room at index %d with a corner hallway."), i);
        }

        continue;

        if (IsCornerRoom(Rooms[i]))
        {
            ASDungeonRoom* HallwayCorner = GetWorld()->SpawnActor<ASDungeonRoom>(CornerHallwayClass, Rooms[i]->GetActorLocation(), Rooms[i]->GetActorRotation());
            const FVector2D* Cell = RoomGrid.FindKey(Rooms[i]);
            if (Cell == nullptr) continue;

            Rooms[i]->Destroy();

            Rooms[i] = HallwayCorner;
            RoomGrid[*Cell] = HallwayCorner;
        }
    }
}

void ASDungeonGenerationComponent::CheckIsIndexCorner(TArray<ASDungeonRoom*>& Rooms, const int32& Index)
{
    if (Rooms.Num() <= 0 || CornerHallwayClass == nullptr) return;

    if (Rooms[Index] == nullptr) return;

    if (Rooms[Index]->GetIsHallway() == false) return;

    if (IsCornerRoom(Rooms[Index]))
    {
        ASDungeonRoom* HallwayCorner = GetWorld()->SpawnActor<ASDungeonRoom>(CornerHallwayClass, Rooms[Index]->GetActorLocation(), Rooms[Index]->GetActorRotation());
        const FVector2D* Cell = RoomGrid.FindKey(Rooms[Index]);
        if (Cell == nullptr) return;

        Rooms[Index]->Destroy();

        Rooms[Index] = HallwayCorner;
        RoomGrid[*Cell] = HallwayCorner;
    }
}

TArray<FVector2D> ASDungeonGenerationComponent::GetPossibleNeighborCells(const FVector2D& CurrentCell)
{
    TArray<FVector2D> NeighborCells;

    // Check Right Cell
    if (RoomGrid.Contains(FVector2D(CurrentCell.X + 1, CurrentCell.Y)))
    {
        if(RoomGrid[FVector2D(CurrentCell.X + 1, CurrentCell.Y)] != nullptr)
            NeighborCells.Add(FVector2D(CurrentCell.X + 1, CurrentCell.Y));
    }
    
    // Check Left Cell
    if (RoomGrid.Contains(FVector2D(CurrentCell.X - 1, CurrentCell.Y)))
    {
        if(RoomGrid[FVector2D(CurrentCell.X - 1, CurrentCell.Y)] != nullptr)
            NeighborCells.Add(FVector2D(CurrentCell.X - 1, CurrentCell.Y));
    }

    // Check Up Cell
    if (RoomGrid.Contains(FVector2D(CurrentCell.X, CurrentCell.Y + 1)))
    {
        if(RoomGrid[FVector2D(CurrentCell.X, CurrentCell.Y + 1)] != nullptr)
            NeighborCells.Add(FVector2D(CurrentCell.X, CurrentCell.Y + 1));
    }

    // Check Down Cell
    if (RoomGrid.Contains(FVector2D(CurrentCell.X, CurrentCell.Y - 1)))
    {
        if(RoomGrid[FVector2D(CurrentCell.X, CurrentCell.Y - 1)] != nullptr)
            NeighborCells.Add(FVector2D(CurrentCell.X, CurrentCell.Y - 1));
    }

    return NeighborCells;
}

TArray<FVector2D> ASDungeonGenerationComponent::GetPossibleEmptyNeighborCells(const FVector2D& CurrentCell)
{
    TArray<FVector2D> NeighborCells;

    // Check Right Cell
    if (RoomGrid.Contains(FVector2D(CurrentCell.X + 1, CurrentCell.Y)))
    {
        if (RoomGrid[FVector2D(CurrentCell.X + 1, CurrentCell.Y)] == nullptr)
            NeighborCells.Add(FVector2D(CurrentCell.X + 1, CurrentCell.Y));
    }

    // Check Left Cell
    if (RoomGrid.Contains(FVector2D(CurrentCell.X - 1, CurrentCell.Y)))
    {
        if (RoomGrid[FVector2D(CurrentCell.X - 1, CurrentCell.Y)] == nullptr)
            NeighborCells.Add(FVector2D(CurrentCell.X - 1, CurrentCell.Y));
    }

    // Check Up Cell
    if (RoomGrid.Contains(FVector2D(CurrentCell.X, CurrentCell.Y + 1)))
    {
        if (RoomGrid[FVector2D(CurrentCell.X, CurrentCell.Y + 1)] == nullptr)
            NeighborCells.Add(FVector2D(CurrentCell.X, CurrentCell.Y + 1));
    }

    // Check Down Cell
    if (RoomGrid.Contains(FVector2D(CurrentCell.X, CurrentCell.Y - 1)))
    {
        if (RoomGrid[FVector2D(CurrentCell.X, CurrentCell.Y - 1)] == nullptr)
            NeighborCells.Add(FVector2D(CurrentCell.X, CurrentCell.Y - 1));
    }

    return NeighborCells;
}

void ASDungeonGenerationComponent::GenerateBranches(TArray<ASDungeonRoom*> Path)
{
    if (Path.Num() <= 0) return;

    for (ASDungeonRoom* Room : Path) 
    {
        if (IsValid(Room) && Room->GetIsHallway() == false) 
        {            
            const FVector2D* Cell = RoomGrid.FindKey(Room);
            if (Cell == nullptr) return;

            // Use the new method to pick a random empty cell
            FVector2D RandomCell = PickRandomEmptyCell();

            // Check if a valid cell was found
            if (RandomCell.X == -1 && RandomCell.Y == -1)
            {
                // Failed to find a valid empty cell, skip this room
                UE_LOG(LogTemp, Warning, TEXT("I HAVE FAILED MY TAKS :("));

                continue;
            }

            UE_LOG(LogTemp, Warning, TEXT("I AM REACHING THIS PART"));
            WalkTowardsEnd(*Cell, RandomCell, 3, 1);
        }
    }
}

void ASDungeonGenerationComponent::GenerateDeadEnds()
{
    if (AllRooms.Num() == 0 || IsValid(DeadendHallwayClass) == false) return;

    for (ASDungeonRoom* Room : AllRooms) 
    {
        if (Room == nullptr) continue;

        if (Room->GetIsHallway()) continue;

        UE_LOG(LogTemp, Warning, TEXT("I AM A POSSIBLE MATCH FOR DEAD ENDS"));

        FVector2D CurrentCell = GetCellPositionFromRoom(Room);

        TArray<FVector2D> EmptyNeighbors;
        EmptyNeighbors.Append(GetPossibleEmptyNeighborCells(CurrentCell));
        if (EmptyNeighbors.Num() == 0) continue;

        for (FVector2D Cell : EmptyNeighbors)
        {
            // IS THIS TRULY EMPTY?
            if (RoomGrid[Cell] != nullptr) 
            {
                UE_LOG(LogTemp, Warning, TEXT("I LIED, I AM NOT TRUELY EMPTY, HAHA GOT YOUR ASS"));

                continue;
            }
            ASDungeonRoom* DeadendHallway = GetWorld()->SpawnActor<ASDungeonRoom>(DeadendHallwayClass, FVector(Cell.X * TileSize, Cell.Y * TileSize, -100.0f), FRotator::ZeroRotator);
            UE_LOG(LogTemp, Warning, TEXT("I MAKE DEADEND, I WORK NOW"));
            if (DeadendHallway == nullptr) return;

            RoomGrid[Cell] = DeadendHallway;

            // Rotate this hallway properly
            FRotator TargetRotation = CalaculateRotationFromRoomPosition(Room, DeadendHallway);

            FQuat TargetQuat;
            TargetQuat = FQuat::MakeFromRotator(TargetRotation);
            DeadendHallway->SetActorRotation(TargetQuat);
        }

    }
}

FVector2D ASDungeonGenerationComponent::GetCellPositionFromRoom(ASDungeonRoom* TargetRoom)
{
    const FVector2D* Cell = RoomGrid.FindKey(TargetRoom);
    if (Cell == nullptr) return FVector2D::ZeroVector;

    return *Cell;
}

FVector2D ASDungeonGenerationComponent::PickRandomEmptyCell()
{
    // Define the cells to exclude
    TArray<FVector2D> ExcludedCells = {
        FVector2D(0, 0),   // Start Room
        FVector2D(5, 5),   // Boss Room cells
        FVector2D(5, 6),
        FVector2D(6, 5),
        FVector2D(6, 6)
    };

    // Divide the grid into regions (quadrants)
    TArray<FVector2D> TopLeftRegion, TopRightRegion, BottomLeftRegion, BottomRightRegion;

    for (int32 X = 0; X < GridSize; ++X)
    {
        for (int32 Y = 0; Y < GridSize; ++Y)
        {
            FVector2D Cell(X, Y);

            // Skip cells that are either occupied or excluded
            if ((RoomGrid.Contains(Cell) && IsValid(RoomGrid[Cell])) || ExcludedCells.Contains(Cell))
            {
                continue;
            }

            // Sort cells into regions
            if (X <= 2 && Y <= 2)
                TopLeftRegion.Add(Cell);
            else if (X >= 3 && Y <= 2)
                TopRightRegion.Add(Cell);
            else if (X <= 2 && Y >= 3)
                BottomLeftRegion.Add(Cell);
            else if (X >= 3 && Y >= 3)
                BottomRightRegion.Add(Cell);
        }
    }

    // Calculate room densities in each region
    int32 TotalCells = GridSize * GridSize;
    int32 TopLeftOccupied = TopLeftRegion.Num();
    int32 TopRightOccupied = TopRightRegion.Num();
    int32 BottomLeftOccupied = BottomLeftRegion.Num();
    int32 BottomRightOccupied = BottomRightRegion.Num();

    float TotalWeight = TopLeftOccupied + TopRightOccupied + BottomLeftOccupied + BottomRightOccupied;

    // If no regions are available, return an invalid vector (safety check)
    if (TotalWeight == 0.0f)
    {
        return FVector2D(-1, -1);  // Invalid position, no empty cells available
    }

    // Weighted random selection based on room densities
    float RandomValue = FMath::FRandRange(0, TotalWeight);

    // Bias the random selection based on the room densities
    if (RandomValue < TopLeftOccupied)
    {
        return PickRandomCellFromRegion(TopLeftRegion);
    }
    else if (RandomValue < TopLeftOccupied + TopRightOccupied)
    {
        return PickRandomCellFromRegion(TopRightRegion);
    }
    else if (RandomValue < TopLeftOccupied + TopRightOccupied + BottomLeftOccupied)
    {
        return PickRandomCellFromRegion(BottomLeftRegion);
    }
    else
    {
        return PickRandomCellFromRegion(BottomRightRegion);
    }
}

FVector2D ASDungeonGenerationComponent::PickRandomCellFromRegion(const TArray<FVector2D>& Region)
{
    if (Region.Num() == 0)
    {
        return FVector2D(-1, -1);  // Invalid position (no empty space)
    }

    // Pick a random index from the region
    int32 RandomIndex = FMath::RandRange(0, Region.Num() - 1);
    return Region[RandomIndex];
}

//
#pragma endregion

//

void ASDungeonGenerationComponent::InitializeGrid()
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

    FVector BossRoomLocation = FVector(4 * TileSize, 5 * TileSize, -600.0f);
    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(BossRoomClass, BossRoomLocation, FRotator::ZeroRotator);
    if (Room == nullptr) return;

    RoomGrid[FVector2D(5, 5)] = Room;
    RoomGrid[FVector2D(5, 6)] = Room;
    RoomGrid[FVector2D(6, 5)] = Room;
    RoomGrid[FVector2D(6, 6)] = Room;

    StartingRooms[1] = Room;

    UE_LOG(LogTemp, Warning, TEXT("Boss Room placed from (4, 4) to (5, 5)"));
}

#pragma endregion

#pragma region Room Generation Functions

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

ASDungeonRoom* ASDungeonGenerationComponent::SpawnRoom(TSubclassOf<ASDungeonRoom> RoomClass, const FVector& Location)
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

ASDungeonRoom* ASDungeonGenerationComponent::SpawnRoom(TSubclassOf<ASDungeonRoom> RoomClass, const FVector& Location, const FRotator& Rotation)
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


    ASDungeonRoom* Room = GetWorld()->SpawnActor<ASDungeonRoom>(RoomClass, Location, Rotation);
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

#pragma region Helper Functions

bool ASDungeonGenerationComponent::IsCornerRoom(ASDungeonRoom* TargetRoom)
{
    const FVector2D* RoomLocation = RoomGrid.FindKey(TargetRoom);

    if (RoomLocation == nullptr) return false;

    TArray<FVector2D> NeighborCells;
    NeighborCells.Append(GetPossibleNeighborCells(*RoomLocation));

    if (NeighborCells.Num() != 2) return false;

    // If both neighbors aren't on the same X or Y level then we know the neighbors
    // must be adjacent to one another not parallel.
    if (FMath::IsNearlyEqual(NeighborCells[0].X, NeighborCells[1].X, 0.01f) == false 
        && FMath::IsNearlyEqual(NeighborCells[0].Y, NeighborCells[1].Y, 0.01f) == false)
    {
        return true;
    }

    return false;
}

FRotator ASDungeonGenerationComponent::CalculateRoomRotation(ASDungeonRoom* TargetRoom)
{
    FRotator Target = FRotator::ZeroRotator;
    const float Tolerance = 0.01f;  // Tolerance for float comparison

    const FVector2D* RoomLocation = RoomGrid.FindKey(TargetRoom);
    if (RoomLocation == nullptr) return Target;

    TArray<FVector2D> NeighborCells;
    NeighborCells.Append(GetPossibleNeighborCells(*RoomLocation));

    if (NeighborCells.Num() == 0 || TargetRoom->Openings.Num() == 0)
        return Target;

    if (TargetRoom->Openings.Num() == 4)
        return Target;

    if (NeighborCells.Num() == 2 && IsCornerRoom(TargetRoom))
    {
        // Calculate direction vectors relative to current room
        FVector2D FirstNeighborDirection = NeighborCells[0] - *RoomLocation;
        FVector2D SecondNeighborDirection = NeighborCells[1] - *RoomLocation;

        // Normalize the direction vectors
        FirstNeighborDirection.Normalize();
        SecondNeighborDirection.Normalize();

        // Log the directions for debugging
        //UE_LOG(LogTemp, Warning, TEXT("First Neighbor Direction: X=%f, Y=%f"), FirstNeighborDirection.X, FirstNeighborDirection.Y);
        //UE_LOG(LogTemp, Warning, TEXT("Second Neighbor Direction: X=%f, Y=%f"), SecondNeighborDirection.X, SecondNeighborDirection.Y);

        // Function to check if two directions are approximately equal
        auto AreDirectionsEqual = [](const FVector2D& Dir1, const FVector2D& Dir2, float Tolerance) -> bool
        {
            return FMath::IsNearlyEqual(Dir1.X, Dir2.X, Tolerance) && FMath::IsNearlyEqual(Dir1.Y, Dir2.Y, Tolerance);
        };

        // Define the direction vectors for Up, Down, Left, Right
        FVector2D Up(0, 1);
        FVector2D Down(0, -1);
        FVector2D Left(-1, 0);
        FVector2D Right(1, 0);

        // Check for Up + Right
        if ((AreDirectionsEqual(FirstNeighborDirection, Up, Tolerance) && AreDirectionsEqual(SecondNeighborDirection, Right, Tolerance)) ||
            (AreDirectionsEqual(FirstNeighborDirection, Right, Tolerance) && AreDirectionsEqual(SecondNeighborDirection, Up, Tolerance)))
        {
            UE_LOG(LogTemp, Warning, TEXT("Up + Right : Corner!"));
            Target = FRotator(0, -90.0f, 0); // Example rotation for Up + Right
        }
        // Check for Up + Left
        else if ((AreDirectionsEqual(FirstNeighborDirection, Up, Tolerance) && AreDirectionsEqual(SecondNeighborDirection, Left, Tolerance)) ||
            (AreDirectionsEqual(FirstNeighborDirection, Left, Tolerance) && AreDirectionsEqual(SecondNeighborDirection, Up, Tolerance)))
        {
            UE_LOG(LogTemp, Warning, TEXT("Up + Left : Corner!"));
            Target = FRotator(0, 0.0f, 0); // Rotation for Up + Left
        }
        // Check for Down + Right
        else if ((AreDirectionsEqual(FirstNeighborDirection, Down, Tolerance) && AreDirectionsEqual(SecondNeighborDirection, Right, Tolerance)) ||
            (AreDirectionsEqual(FirstNeighborDirection, Right, Tolerance) && AreDirectionsEqual(SecondNeighborDirection, Down, Tolerance)))
        {
            UE_LOG(LogTemp, Warning, TEXT("Down + Right : Corner!"));
            Target = FRotator(0, 180.0f, 0); // Rotation for Down + Right
        }
        // Check for Down + Left
        else if ((AreDirectionsEqual(FirstNeighborDirection, Down, Tolerance) && AreDirectionsEqual(SecondNeighborDirection, Left, Tolerance)) ||
            (AreDirectionsEqual(FirstNeighborDirection, Left, Tolerance) && AreDirectionsEqual(SecondNeighborDirection, Down, Tolerance)))
        {
            UE_LOG(LogTemp, Warning, TEXT("Down + Left : Corner!"));
            Target = FRotator(0, 90.0f, 0); // Rotation for Down + Left
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Unexpected Corner Configuration!"));
        }

        //UE_LOG(LogTemp, Warning, TEXT("First Neighbor Direction: X=%f, Y=%f"), FirstNeighborDirection.X, FirstNeighborDirection.Y);
        //UE_LOG(LogTemp, Warning, TEXT("Second Neighbor Direction: X=%f, Y=%f"), SecondNeighborDirection.X, SecondNeighborDirection.Y);
    }
    else if (NeighborCells.Num() == 2)
    {
        // I am a straight path room
        FVector2D FirstNeighbor = NeighborCells[0] - *RoomLocation;
        FVector2D SecondNeighbor = NeighborCells[1] - *RoomLocation;

        if ((FirstNeighbor == FVector2D(1, 0) && SecondNeighbor == FVector2D(-1, 0)) ||
            (FirstNeighbor == FVector2D(-1, 0) && SecondNeighbor == FVector2D(1, 0))) // Horizontal neighbors
        {
            Target = FRotator(0, 0.0f, 0); // Rotate for horizontal path
        }
        else if ((FirstNeighbor == FVector2D(0, 1) && SecondNeighbor == FVector2D(0, -1)) ||
            (FirstNeighbor == FVector2D(0, -1) && SecondNeighbor == FVector2D(0, 1))) // Vertical neighbors
        {
            Target = FRotator(0, 90.0f, 0); // No rotation for vertical path
        }
    }

    return Target;
}

FRotator ASDungeonGenerationComponent::CalaculateRotationFromRoomPosition(ASDungeonRoom* ParentRoom, ASDungeonRoom* TargetRoom)
{
    FVector2D ParentRoomCell = GetCellPositionFromRoom(ParentRoom);
    FVector2D TargetRoomCell = GetCellPositionFromRoom(TargetRoom);

    FVector2D Direction = TargetRoomCell;

    if (TargetRoomCell.X > ParentRoomCell.X)  // TargetRoom is above ParentRoom (Up)
    {
        return FRotator(0.0f, 0.0f, 0.0f);  // No rotation needed
    }
    else if (TargetRoomCell.X < ParentRoomCell.X)  // TargetRoom is below ParentRoom (Down)
    {
        return FRotator(0.0f, 180.0f, 0.0f);  // Rotate 180 degrees
    }
    else if (TargetRoomCell.Y > ParentRoomCell.Y)  // TargetRoom is to the right of ParentRoom (Right)
    {
        return FRotator(0.0f, 90.0f, 0.0f);  // Rotate 90 degrees
    }
    else if (TargetRoomCell.Y < ParentRoomCell.Y)  // TargetRoom is to the left of ParentRoom (Left)
    {
        return FRotator(0.0f, -90.0f, 0.0f);  // Rotate -90 degrees
    }

    // If the target room is at the same position (no valid direction), return a default rotation
    return FRotator::ZeroRotator;
}

#pragma endregion
