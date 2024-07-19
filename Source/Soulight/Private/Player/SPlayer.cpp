// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/SPlayerController.h"

#include "Animation/AnimInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"

ASPlayer::ASPlayer()
{
	
}

void ASPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void ASPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();

}

void ASPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASPlayer::Move(const FInputActionValue& InputValue)
{

}

void ASPlayer::Aim(const FInputActionValue& InputValue)
{

}

void ASPlayer::Interact()
{

}

void ASPlayer::Attack()
{

}

FVector ASPlayer::GetMoveFwdDir() const
{
	FVector CamerFwd = MainCamera->GetForwardVector();
	CamerFwd.Z = 0;
	return CamerFwd.GetSafeNormal();
}

FVector ASPlayer::GetMoveRightDir() const
{
	return MainCamera->GetRightVector();
}
