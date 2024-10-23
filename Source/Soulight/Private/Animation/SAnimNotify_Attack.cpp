// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SAnimNotify_Attack.h"

#include "Framework/SCharacterBase.h"

void USAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!IsValid(MeshComp)) return;

	Super::Notify(MeshComp, Animation);

    AActor* Owner = MeshComp->GetOwner();
    if (!IsValid(Owner)) return;

    FVector ForwardVector = Owner->GetActorForwardVector();
    FVector AttackLocation = Owner->GetActorLocation() + (ForwardVector * AttackOffset);

    AttackLocation.Z = 50.0f;

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackSize);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);

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
                ASCharacterBase* HitCharacter = Cast<ASCharacterBase>(HitActor);
                if (!IsValid(HitCharacter)) continue;

                UE_LOG(LogTemp, Warning, TEXT("Hit Enemy: %s"), *HitCharacter->GetName());

                // Replace Damage for player/enemy attack damage
                HitCharacter->TakeDamage(AttackDamage);
            }
        }
    }

    // This is wrong for some reason in the editor idk why, editor has meshes facing right not forward
    //DrawDebugSphere(GetWorld(), AttackLocation, AttackSize, 32, FColor::Red, false, 0.2f);
}
