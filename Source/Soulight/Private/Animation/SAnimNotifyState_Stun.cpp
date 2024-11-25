// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SAnimNotifyState_Stun.h"

#include "Framework/SCharacterBase.h"

void USAnimNotifyState_Stun::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    if (!IsValid(MeshComp)) return;

    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

    AActor* Owner = MeshComp->GetOwner();
    if (!IsValid(Owner)) return;

    FVector ForwardVector = Owner->GetActorForwardVector();
    FVector AttackLocation = Owner->GetActorLocation() + (ForwardVector * AttackOffset);

    AttackLocation.Z = 50.0f;

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackSize);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);

    ASCharacterBase* OwningCharacter = Cast<ASCharacterBase>(Owner);
    if (!IsValid(OwningCharacter)) return;

    if (Owner->GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        AttackLocation,
        FQuat::Identity,
        ECC_Pawn,
        CollisionShape,
        QueryParams))
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            if (AActor* HitActor = Result.GetActor())
            {
                if (HitActors.Contains(HitActor)) return;

                ASCharacterBase* HitCharacter = Cast<ASCharacterBase>(HitActor);
                if (!IsValid(HitCharacter)) continue;

                UE_LOG(LogTemp, Warning, TEXT("Hit Enemy: %s"), *HitCharacter->GetName());

                // Replace Damage for player/enemy attack damage
                HitCharacter->ApplyStun(StunDuration, OverrideStun);

                HitActors.Add(HitActor);
            }
        }
    }

    // This is wrong for some reason in the editor idk why, editor has meshes facing right not forward
    DrawDebugSphere(GetWorld(), AttackLocation, AttackSize, 32, FColor::Red, false, 0.2f);
}

void USAnimNotifyState_Stun::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!IsValid(MeshComp))
    {
        HitActors.Empty();
        return;
    }

    Super::NotifyEnd(MeshComp, Animation, EventReference);

    HitActors.Empty();
}
