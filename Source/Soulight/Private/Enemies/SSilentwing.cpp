// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SSilentwing.h"

#include "Components/SFlyingComponent.h"

#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"

#include "Player/SPlayer.h"

ASSilentwing::ASSilentwing()
{
	FlyingComponent = CreateDefaultSubobject<USFlyingComponent>(TEXT("Flying Component"));
}

void ASSilentwing::BeginPlay()
{
	Super::BeginPlay();

	Target = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (FlyRange < AttackRange)
    {
        FlyRange = AttackRange * 2;
    }
}

void ASSilentwing::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!IsValid(Target))
    {
        return;
    }

    float Distance = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());

    if (Distance <= AttackRange && FlyingComponent->GetIsFlying())
    {
        FlyingComponent->StopFly(Target->GetActorLocation());
    }

    if (Distance >= FlyRange && !FlyingComponent->GetIsFlying())
    {
        FlyingComponent->StartFly();
    }
}
