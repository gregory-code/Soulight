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
	const int32 GridSize = 6; 
	const float TileSize = 1500.0f;

	TArray<TArray<bool>> Grid;

	void InitializeGrid();
	void PlaceStartRoom();
	void PlaceBossRoom();

	void GenerateRooms(const int32& NumRooms);

	void GenerateHallways(const int32& Index);
	void SpawnHallways(const FVector& Location, const FRotator& Rotation);
	
	void SpawnRoom(TSubclassOf<ASDungeonRoom> RoomClass, FVector Location);

	void GenerateChests(const int32& NumChests);

	TSubclassOf<ASDungeonRoom> GetRandomRoom();

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

	TArray<ASDungeonRoom*> StartingRooms;

	TArray<ASDungeonRoom*> AllRooms;

};
