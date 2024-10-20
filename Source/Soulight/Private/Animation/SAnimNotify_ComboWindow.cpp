// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SAnimNotify_ComboWindow.h"
#include "GameFramework/Character.h"
#include "Player/SPlayer.h"

void USAnimNotify_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!IsValid(MeshComp)) return;

    if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
    {
        ASPlayer* Player = Cast<ASPlayer>(Character);
        if (!IsValid(Player)) return;

        Player->bHasAttacked = false;

        if (Player->CurrentCombo < Player->ComboSectionMontages.Num() - 1)
        {
            Player->CurrentCombo++;
            Player->bCanAttack = true;
        }
    }
}

void USAnimNotify_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!IsValid(MeshComp)) return;

    if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
    {
        ASPlayer* Player = Cast<ASPlayer>(Character);
        if (IsValid(Player))
        {
            if (Player->bHasAttacked == true) return;

            Player->CurrentCombo = 0;
            Player->EndCombo();
        }
    }
}