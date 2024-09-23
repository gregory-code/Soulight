// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SDungeonGenerationComponent.generated.h"

class ASDungeonRoom;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USDungeonGenerationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USDungeonGenerationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	const int32 GridSize = 6;  // 6x6 grid
	const float TileSize = 1500.0f;

	TArray<TArray<bool>> Grid; // 2D array representing the grid (0 = empty, 1 = start room, 2 = boss room)

	void InitializeGrid();
	void PlaceStartRoom();
	void PlaceBossRoom();

	void GenerateRooms(const int32& NumRooms);

	void GenerateHallways(const int32& Index);
	void SpawnHallways(const FVector& Location, const FRotator& Rotation);
	
	void SpawnRoom(TSubclassOf<ASDungeonRoom> RoomClass, FVector Location);

	void GenerateChests(const int32& NumChests);

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> StartingRoomClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TArray<TSubclassOf<ASDungeonRoom>> RoomClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> BossRoomClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> HallwayClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<AActor> ChestClass;
	TArray<FTransform> ChestSpawnPoints;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	int32 MaxNumChests = 0;

	TArray<ASDungeonRoom*> InstancedRooms;

/*
* 
private:
	///////////////////////////////////////////
	//         Dungeon Generation           //
	/////////////////////////////////////////

	const float GridOffset = 1500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	FVector2D GridSize;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> StartingRoomClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> BossRoomClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<AActor> ChestClass;
	TArray<FTransform> ChestSpawnPoints;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	int32 MaxNumChests = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	int32 MaxNumRoom = 0;

	void DepthFirstSearch(ASDungeonRoom* CurrentRoom, ASDungeonRoom* EndRoom, int32& RoomCount, const int32& NumRooms);

	void GenerateDungeon(const int32& NumRooms);
	ASDungeonRoom* GenerateRoom(TSubclassOf<ASDungeonRoom> RoomClass, const FVector& Position);

	void GenerateChests(const int32& NumChests);

	bool IsRoomAlreadyGenerated(const FVector& RoomPosition);
	
	TArray<FVector> TryGetPossibleDirections(ASDungeonRoom* CurrentRoom);

	void RotateRoomBasedOnDirection(ERoomOpeningDirection Direction, ASDungeonRoom* Room);

	TArray<FVector> PossibleDirections = {
		FVector(0, GridOffset, 0),
		FVector(0, -GridOffset, 0),
		FVector(GridOffset, 0, 0),
		FVector(-GridOffset, 0, 0)
	};
*/

};
