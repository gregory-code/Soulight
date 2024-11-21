// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_SimpleAttack.h"

#include "AIController.h"

#include "Framework/SCharacterBase.h"

EBTNodeResult::Type USBTTask_SimpleAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!IsValid(AIC)) return EBTNodeResult::Failed;

	APawn* OwningPawn = AIC->GetPawn();
	if (!IsValid(OwningPawn)) return EBTNodeResult::Failed;

    FVector ForwardVector = OwningPawn->GetActorForwardVector();
    FVector AttackLocation = OwningPawn->GetActorLocation() + (ForwardVector * AttackOffset);

    AttackLocation.Z = 50.0f;

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackSize);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwningPawn);

    if (OwningPawn->GetWorld()->OverlapMultiByChannel(
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
                HitCharacter->TakeDamage(AttackDamage, OwningPawn, Knockback);
            }
        }
    }

    // This is wrong for some reason in the editor idk why, editor has meshes facing right not forward
    DrawDebugSphere(GetWorld(), AttackLocation, AttackSize, 32, FColor::Red, false, 0.2f);

	return EBTNodeResult::Succeeded;
}
