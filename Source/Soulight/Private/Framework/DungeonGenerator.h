// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

class ASDungeonRoom;

UCLASS()
class ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
private:
	TArray<ASDungeonRoom*> GenerateMainPath(ASDungeonRoom* RootRoom, const FVector2D& StartPosition, const FVector2D& EndPosition, const int32& Steps, const int32& NumRoomsToKeep);

private:
	void InitializeGrid();
	void PlaceStartRoom();
	void PlaceBossRoom();

private:
	const int32 GridSize = 6;
	const float TileSize = 1500.0f;

	TMap<FVector2D, ASDungeonRoom*> RoomGrid;

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
	TSubclassOf<ASDungeonRoom> DeadendHallwayClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TMap<int32, TSubclassOf<ASDungeonRoom>> RoomMap;

	TArray<ASDungeonRoom*> StartingRooms;

	TArray<ASDungeonRoom*> HallwayList;

	TArray<ASDungeonRoom*> AllRooms;
	*/
};
