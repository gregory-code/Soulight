// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SDreadstorm.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Widget.h"

#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"

#include "Player/SPlayer.h"

#include "Widgets/SHealthbar.h"

ASDreadstorm::ASDreadstorm()
{
	HeadHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Head Hurtbox"));
	HeadHurtbox->SetupAttachment(GetRootComponent());

	LeftHandHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand Hitbox"));
	LeftHandHitbox->SetupAttachment(GetRootComponent());

	RightHandHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand Hitbox"));
	RightHandHitbox->SetupAttachment(GetRootComponent());
}

void ASDreadstorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerTarget) return;

	if (bTransitioning) return;

	if (MovesTillNextHeadMovement == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Head State Changing"));

		bHeadRaised ? LowerHead() : RaiseHead();

		MovesTillNextHeadMovement = FMath::RandRange(1, 3);
	}

	if (!GetMesh()->GetAnimInstance()) return;

	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return; // Attack Currently In Progress
	}

	SelectAttack();
}

void ASDreadstorm::BeginPlay()
{
	Super::BeginPlay();

	HealthBar = CreateWidget<USHealthbar>(GetWorld(), HealthBarWidget);
	HealthBar->AddToViewport();

	if (IsValid(GetMesh()))
	{
		if (GetMesh()->DoesSocketExist(LeftHandSocketName))
		{
			LeftHandHitbox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandSocketName);
		}

		if (GetMesh()->DoesSocketExist(RightHandSocketName))
		{
			RightHandHitbox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandSocketName);
		}
	}
	
	if (IsValid(HealthBar)) 
	{
		// Force a layout prepass to ensure the widget's components are ready
		HealthBar->SynchronizeProperties();
		HealthBar->ForceLayoutPrepass();

		HealthBar->UpdateHealthbar(Health, MaxHealth);
	}

	if (StartMontage)
	{
		PlayAnimMontage(StartMontage);
	}

	RightHandHitbox->OnComponentBeginOverlap.AddDynamic(this, &ASDreadstorm::RightHandOverlapBegin);
	RightHandHitbox->OnComponentEndOverlap.AddDynamic(this, &ASDreadstorm::RightHandOverlapEnd);

	LeftHandHitbox->OnComponentBeginOverlap.AddDynamic(this, &ASDreadstorm::LeftHandOverlapBegin);
	LeftHandHitbox->OnComponentEndOverlap.AddDynamic(this, &ASDreadstorm::LeftHandOverlapEnd);

	FTimerHandle LateSetupTimer;
	GetWorld()->GetTimerManager().SetTimer(LateSetupTimer, this, &ASDreadstorm::LateSetup, 1.f/24.f, false, 2.f);
}

void ASDreadstorm::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (!MontageToPlay || !GetMesh() || !GetMesh()->GetAnimInstance()) return;

	GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
}

void ASDreadstorm::StopAllMontages()
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance()) return;

	GetMesh()->GetAnimInstance()->StopAllMontages(1.f);
}

void ASDreadstorm::LateSetup()
{
	PlayerTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerTarget)
	{
		NewPlayerLocation = PlayerTarget->GetActorLocation();
	}
}

bool ASDreadstorm::InRange(const FVector& CurrentLocation, const FVector& PlayerLocation) const
{
	float Distance = FVector::Distance(CurrentLocation, PlayerLocation);

	return Distance < 400.f;
}

void ASDreadstorm::RightHandOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHitOnce || !OtherActor) return;

	ASPlayer* Target = Cast<ASPlayer>(OtherActor);
	if (Target)
	{
		DealDamage(Target);

		bHitOnce = true;
	}
}

void ASDreadstorm::RightHandOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	UE_LOG(LogTemp, Warning, TEXT("Right Hand Overlapping"));

	ASPlayer* Target = Cast<ASPlayer>(OtherActor);
	if (Target)
	{
		bHitOnce = false;
	}
}

void ASDreadstorm::LeftHandOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHitOnce || !OtherActor) return;

	UE_LOG(LogTemp, Warning, TEXT("Left Hand Overlapping"));

	ASPlayer* Target = Cast<ASPlayer>(OtherActor);
	if (Target)
	{
		DealDamage(Target);

		bHitOnce = true;
	}
}

void ASDreadstorm::LeftHandOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	ASPlayer* Target = Cast<ASPlayer>(OtherActor);
	if (Target)
	{
		bHitOnce = false;
	}
}

void ASDreadstorm::DealDamage(ASCharacterBase* TargetCharacter)
{
	if (!TargetCharacter) return;

	TargetCharacter->CharacterTakeDamage(15.f, this, 3.f);
}

void ASDreadstorm::CharacterTakeDamage(float Damage, AActor* DamageInstigator, const float& Knockback)
{
	Super::CharacterTakeDamage(Damage, DamageInstigator, 0);

	if (!IsValid(HealthBar)) return;

	CheckPhaseState();

	if (Health <= 0)
	{
		HealthBar->RemoveFromViewport();
	}

	HealthBar->UpdateHealthbar(Health, MaxHealth);
}

void ASDreadstorm::SelectAttack()
{
	if (!bCanAttack) return;

	bCanAttack = false;

	MovesTillNextHeadMovement--;

	UE_LOG(LogTemp, Warning, TEXT("Range: %f"), FVector::Distance(GetActorLocation(), PlayerTarget->GetActorLocation()));

	if (InRange(GetActorLocation(), PlayerTarget->GetActorLocation()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Claw Attack"));

		PlayMontage(ClawAttackAnimation);

		float WaitTime = ClawAttackAnimation->GetPlayLength() + (ClawAttackAnimation->GetPlayLength()/2);
		GetWorld()->GetTimerManager().SetTimer(AttackResetTimer, this, &ASDreadstorm::WaitForNextAttack, 1.f/24.f, false, WaitTime);
	}
	else 
	{
		if (RangedAttackAnimations.Num() == 0) return;

		int32 RandomMontage = FMath::RandRange(0, RangedAttackAnimations.Num() - 1);

		UE_LOG(LogTemp, Warning, TEXT("Ranged Attack"));

		PlayMontage(RangedAttackAnimations[RandomMontage]);

		float WaitTime = RangedAttackAnimations[RandomMontage]->GetPlayLength() + (RangedAttackAnimations[RandomMontage]->GetPlayLength() / 2);
		GetWorld()->GetTimerManager().SetTimer(AttackResetTimer, this, &ASDreadstorm::WaitForNextAttack, 1.f / 24.f, false, WaitTime);
	}
}

void ASDreadstorm::WaitForNextAttack()
{
	bCanAttack = true;
}

void ASDreadstorm::CheckPhaseState()
{
	if (bPhaseTwo == false && Health > 300 && Health <= 600) // Phase 2
	{
		HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bTransitioning = true;

		bPhaseTwo = true;

		StopAllMontages();
		PlayMontage(DigDownMontage);

		GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &ASDreadstorm::DragonTransitioningState, 1.0f, false, TransitionDuration);
	}
	else if (bPhaseThree == false && Health <= 300) // Phase 3 
	{
		HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bTransitioning = true;

		bPhaseTwo = false;
		bPhaseThree = true;

		StopAllMontages();
		PlayMontage(DigDownMontage);

		// Double the duration idk, if this is too easy for player make more fireballs
		GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &ASDreadstorm::DragonTransitioningState, 1.0f, false, TransitionDuration * 2);
	}
	// else I am in phase 1

}

void ASDreadstorm::DragonTransitioningState()
{
	HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetVisibility(true);
	bTransitioning = false;

	PlayMontage(EmergeMontage);

	if (bPhaseTwo)
	{
		FRotator TargetRotation = FRotator(0.0f, 90.0f, 0.0f);
		FQuat RotationQuat = FQuat(TargetRotation);

		SetActorLocationAndRotation(LeftStagePosition, RotationQuat, false, nullptr, ETeleportType::None);
	}
	else if (bPhaseThree)
	{
		FRotator TargetRotation = FRotator(0.0f, -90.0f, 0.0f);
		FQuat RotationQuat = FQuat(TargetRotation);

		SetActorLocationAndRotation(RightStagePosition, RotationQuat, false, nullptr, ETeleportType::None);
	}
}

void ASDreadstorm::SetRightHandHitboxEnabled(bool ActiveState)
{
	if (ActiveState)
	{
		RightHandHitbox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		RightHandHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASDreadstorm::SetLeftHandHitboxEnabled(bool ActiveState)
{
	if (ActiveState) 
	{
		LeftHandHitbox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		LeftHandHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool ASDreadstorm::GetIsDead()
{
	return Health <= 0;
}

void ASDreadstorm::RaiseHead()
{
	HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bHeadRaised = true;
}

void ASDreadstorm::LowerHead()
{
	HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bHeadRaised = false;
}




