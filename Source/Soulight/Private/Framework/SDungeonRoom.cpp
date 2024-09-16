// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/SDungeonRoom.h"

ASDungeonRoom::ASDungeonRoom()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASDungeonRoom::Init(ASDungeonRoom* ParentRoom)
{
	Parent = ParentRoom;

	if (ParentRoom != nullptr) 
	{
		ParentRoom->AddChildRoom(this);
	}
}

void ASDungeonRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASDungeonRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASDungeonRoom::AddChildRoom(ASDungeonRoom* ChildRoom)
{
	ChildrenRoom.Add(ChildRoom);
}

void ASDungeonRoom::RemoveChildRoom(ASDungeonRoom* RoomToRemove)
{
	ChildrenRoom.Remove(RoomToRemove);
}

