// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "SDungeonRoom.generated.h"

UENUM(BlueprintType)
enum class ERoomOpeningDirection : uint8
{
	Up			UMETA(DisplayName = "Up"),
	Down		UMETA(DisplayName = "Down"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};

UCLASS()
class ASDungeonRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	ASDungeonRoom();

	void Init(ASDungeonRoom* ParentRoom);

	bool bVisited = false;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void AddChildRoom(ASDungeonRoom* ChildRoom);
	void RemoveChildRoom(ASDungeonRoom* RoomToRemove);

	TArray<FTransform> GetChestSpawnPoints();

	UPROPERTY(EditDefaultsOnly, Category = "Room Settings")
	TArray<ERoomOpeningDirection> Openings;

	FORCEINLINE TArray<ASDungeonRoom*> GetChildrenRoom() { return ChildrenRoom; }

	FORCEINLINE bool GetUpOpening() { return UpOpening; }
	FORCEINLINE bool GetDownOpening() { return DownOpening; }
	FORCEINLINE bool GetRightOpening() { return RightOpening; }
	FORCEINLINE bool GetLeftOpening() { return LeftOpening; }


private:
	UPROPERTY(EditDefaultsOnly, Category = "Room Details")
	bool UpOpening;

	UPROPERTY(EditDefaultsOnly, Category = "Room Details")
	bool DownOpening;

	UPROPERTY(EditDefaultsOnly, Category = "Room Details")
	bool RightOpening;

	UPROPERTY(EditDefaultsOnly, Category = "Room Details")
	bool LeftOpening;

	UPROPERTY(VisibleAnywhere, Category = "Room Details")
	TArray<USceneComponent*> ChestSpawnPoint;

	TArray<ASDungeonRoom*> ChildrenRoom;

	ASDungeonRoom* Parent;

	int32 TreeLayerIndex;

};
