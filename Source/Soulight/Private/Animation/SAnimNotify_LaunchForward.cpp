// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SAnimNotify_LaunchForward.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/Character.h"


void USAnimNotify_LaunchForward::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

    AActor* Owner = MeshComp->GetOwner();
    if (!IsValid(Owner)) return;

    ACharacter* Character = Cast<ACharacter>(Owner);
    if (!IsValid(Character)) return;

    Character->GetCharacterMovement()->Launch(Character->GetActorForwardVector() * Force);
}
