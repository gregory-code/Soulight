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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	///////////////////////////////////////////
	/*         Dungeon Generation           */
	/////////////////////////////////////////

	const float GridOffset = 1500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	FVector2D GridSize;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> StartingRoomClass;

	UPROPERTY(EditDefaultsOnly, Category = "Dungeon Settings")
	TSubclassOf<ASDungeonRoom> BossRoomClass;

	void DepthFirstSearch(ASDungeonRoom* CurrentRoom, ASDungeonRoom* EndRoom, int32& RoomCount, const int32& NumRooms);

	void GenerateDungeon(const int32& NumRooms);
	ASDungeonRoom* GenerateRoom(TSubclassOf<ASDungeonRoom> RoomClass, const FVector& Position);

	bool IsRoomAlreadyGenerated(const FVector& RoomPosition);
	void Shuffle(TArray<FVector>& Array);
};
