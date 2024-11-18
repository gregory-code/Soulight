// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SProjectileBase.h"

#include "Framework/SCharacterBase.h"

#include "Components/SphereComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"

ASProjectileBase::ASProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereCollider->SetupAttachment(RootComponent);

	SphereCollider->SetGenerateOverlapEvents(true);
	SphereCollider->SetCollisionProfileName(TEXT("OverlapAll"));

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	ProjectileComponent->InitialSpeed = Velocity;
	ProjectileComponent->MaxSpeed = 10000.0f;
	ProjectileComponent->ProjectileGravityScale = 0.0f;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->bShouldBounce = false;
}

void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileComponent->InitialSpeed = Velocity;

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnOverlapBegin);
}

void ASProjectileBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap with owner detected, ignoring."));
		return;
	}

	if (OtherActor->IsA(TargetClass))
	{
		ASCharacterBase* HitCharacter = Cast<ASCharacterBase>(OtherActor);
		if (IsValid(HitCharacter))
			HitCharacter->TakeDamage(Damage);

		Destroy();
	}
}

