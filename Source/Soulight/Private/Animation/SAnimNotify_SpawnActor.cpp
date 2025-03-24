// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SAnimNotify_SpawnActor.h"

void USAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp)) return;

	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner)) return;

	FVector ForwardVector = Owner->GetActorForwardVector();
	FVector SpawnLocation = Owner->GetActorLocation() + (ForwardVector * AttackOffset);

	SpawnLocation.Z = 50.0f;

	if (IsValid(ActorToSpawn)) 
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Owner;

		GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, Owner->GetActorRotation(), SpawnParams);
	}

	//DrawDebugSphere(GetWorld(), SpawnLocation, 1.0f, 32, FColor::Red, false, 0.2f);
}
