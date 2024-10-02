// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDungeonGenerationComponent.generated.h"

class ASDungeonRoom;

UCLASS()
class ASDungeonGenerationComponent : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ASDungeonGenerationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void WalkTowardsEnd(const int32& Steps);
	//bool IsRoomValid(const FVector2D& Position);
	FVector2D GetRandomMoveDirection(const FVector2D& CurrentPos, const FVector2D& BossRoomPosition, float& OutRotation);
	TArray<ASDungeonRoom*> WalkingGeneration(const int32& Steps, const FVector2D& StartingPosition, const FVector2D& EndPosition);
	void ReplaceRoomsWithHallways(TArray<ASDungeonRoom*>& Rooms, const int32& NumRoomsToKeep);

	void CheckForCorners(TArray<ASDungeonRoom*>& Rooms);
	void CheckIsIndexCorner(TArray<ASDungeonRoom*>& Rooms, const int32& Index);

	TArray<FVector2D> GetPossibleNeighborCells(const FVector2D& CurrentCell);

	//bool TryMove(FVector2D& CurrentPosition, const FVector2D& MoveDirection);
	//void ConnectToBossRoom(FVector2D& CurrentPosition, const FVector2D& BossRoomPosition);

private:
	const int32 GridSize = 6; 
	const float TileSize = 1500.0f;

	TMap<FVector2D, ASDungeonRoom*> RoomGrid;

	void InitializeGrid();
	void PlaceStartRoom();
	void PlaceBossRoom();

	void SpawnHallway(const int32& RoomIndex, const int32& HallwayIndex, const FVector& Location, const FRotator& Rotation);
	
	ASDungeonRoom* SpawnRoom(TSubclassOf<ASDungeonRoom> RoomClass, const FVector& Location);
	ASDungeonRoom* SpawnRoom(TSubclassOf<ASDungeonRoom> RoomClass, const FVector& Location, const FRotator& Rotation);

	void GenerateChests(const int32& NumChests);

	bool IsCornerRoom(ASDungeonRoom* TargetRoom);

	FRotator CalculateRoomRotation(ASDungeonRoom* TargetRoom);

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> StartingRoomClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> CornerRoomClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> BossRoomClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> HallwayClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> CornerHallwayClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TMap<int32, TSubclassOf<ASDungeonRoom>> RoomMap;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<AActor> ChestClass;
	TArray<FTransform> ChestSpawnPoints;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	int32 MaxNumChests = 0;

	TArray<ASDungeonRoom*> StartingRooms;

	TArray<ASDungeonRoom*> HallwayList;

	TArray<ASDungeonRoom*> AllRooms;

};
