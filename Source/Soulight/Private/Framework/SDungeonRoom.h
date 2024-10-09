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

	UFUNCTION()
	bool GetIsHallway();

	UFUNCTION()
	bool GetIsCorner();

	FORCEINLINE TArray<ASDungeonRoom*> GetChildrenRoom() { return ChildrenRoom; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Room Details")
	TArray<USceneComponent*> ChestSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Room Details")
	bool bIsHallway = false;

	UPROPERTY(EditDefaultsOnly, Category = "Room Details")
	bool bIsCorner = false;

	TArray<ASDungeonRoom*> ChildrenRoom;

	ASDungeonRoom* Parent;

	int32 TreeLayerIndex;

};
