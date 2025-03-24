// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SAnimNotifyState_InRange.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"

void USAnimNotifyState_InRange::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Target = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	OwningActor = Cast<AActor>(MeshComp->GetOwner());
}

void USAnimNotifyState_InRange::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	IsTargetInRange(OwningActor);
}

bool USAnimNotifyState_InRange::IsTargetInRange(AActor* OwnerActor) const
{
	if (Target)
	{
		float Distance = FVector::Distance(OwnerActor->GetActorLocation(), Target->GetActorLocation());
		return Distance > MinDistance && Distance < MaxDistance;
	}
	
	return false;
}
