// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SProjectileBase.h"

#include "Framework/SCharacterBase.h"

#include "Components/SphereComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"

ASProjectileBase::ASProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));

	RootComponent = SphereCollider;

	SphereCollider->SetGenerateOverlapEvents(true);
	SphereCollider->SetCollisionProfileName(TEXT("OverlapAll"));

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	ProjectileComponent->InitialSpeed = Velocity;
	ProjectileComponent->MaxSpeed = 10000.0f;
	ProjectileComponent->ProjectileGravityScale = 0.0f;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->bShouldBounce = false;

	ProjectileComponent->bIsHomingProjectile = false;
	ProjectileComponent->HomingAccelerationMagnitude = 20000.0f;
}

void ASProjectileBase::EnableOrbit(AActor* OrbitTarget)
{
	if (!IsValid(OrbitTarget)) return;

	bShouldOrbit = true;

	OrbitRadius = 150.0f;

	SetActorTickEnabled(true);

	OrbitCenter = OrbitTarget;
}

void ASProjectileBase::DisableOrbit()
{
	bShouldOrbit = false;
}

void ASProjectileBase::LaunchAtTarget(AActor* Target)
{
	if (!IsValid(Target))
	{
		UE_LOG(LogTemp, Error, TEXT("LaunchAtTarget: Target is invalid!"));
		return;
	}

	// Detach to remove any inherited transformations
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	bShouldOrbit = false;
	OrbitCenter = nullptr;

	//ProjectileComponent->bIsHomingProjectile = true;
	//ProjectileComponent->HomingTargetComponent = Target->GetRootComponent();

	//FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	//ProjectileComponent->Velocity = Direction * 10000.0f;

	TargetActor = Target;
	SetActorTickEnabled(true);

	//UE_LOG(LogTemp, Warning, TEXT("Homing towards: %s | Direction: %s"), *Target->GetName(), *Direction.ToString());
}

void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileComponent->InitialSpeed = Velocity;

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnOverlapBegin);
}

void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldOrbit && IsValid(OrbitCenter))
	{
		OrbitAngle += OrbitSpeed * DeltaTime;
		FVector OrbitOffset = FVector(FMath::Cos(OrbitAngle) * OrbitRadius, FMath::Sin(OrbitAngle) * OrbitRadius, 10.0f);
		SetActorLocation(OrbitCenter->GetActorLocation() + OrbitOffset);
	}

	if (IsValid(TargetActor))
	{
		// Move smoothly toward the target
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetActor->GetActorLocation(), DeltaTime, LerpSpeed);
		SetActorLocation(NewLocation);
	}
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
			HitCharacter->CharacterTakeDamage(Damage, this, Knockback);

		Destroy();
	}
}

