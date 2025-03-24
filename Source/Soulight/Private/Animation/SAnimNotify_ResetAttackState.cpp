// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SAnimNotify_ResetAttackState.h"

#include "Player/SPlayer.h"

void USAnimNotify_ResetAttackState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!IsValid(MeshComp)) return;

    Super::Notify(MeshComp, Animation, EventReference);

    AActor* Owner = MeshComp->GetOwner();
    if (!IsValid(Owner)) return;

    ASPlayer* OwningCharacter = Cast<ASPlayer>(Owner);
    if (IsValid(OwningCharacter))
    {
        OwningCharacter->EndCombo();
    }
}
