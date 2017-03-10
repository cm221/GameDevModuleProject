// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDevGroupModule.h"
#include "EnemyCharacter.h"
#include "ProjectileDamageType.h"
#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Volume"));
	AttackVolume->SetupAttachment(GetCapsuleComponent());
	AttackVolume->RelativeLocation = FVector(60.f, 0.0f, 0.0f); // Position the attack volume
	AttackVolume->SetBoxExtent(FVector(60.0f, 40.0f, 80.f), true);
	AttackVolume->bGenerateOverlapEvents = true;
	AttackVolume->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapBegin);
	AttackVolume->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialising health value
	CurrentHealth = InitialHealth;
	
}

// Called every frame
void AEnemyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

// Called by the engine when damage is dealt
float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
class AController * EventInstigator, AActor * DamageCauser)
{
	// Convert damage amount to integer & clamp it between 0 & the current health value
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	// Update current health value
	CurrentHealth -= DamageToApply;

	// Check if dead
	IsDeadCheck();

	// Check if damage done is by projectile damage type
	if (Cast<UProjectileDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()))
	{
		//Damage done by projectile;
	}

	return DamageToApply;
}

// Check if enemy is dead
void AEnemyCharacter::IsDeadCheck()
{
	if (CurrentHealth <= 0)
	{
		isDead = true;
		IsDeadTrue();
	}
	else
		isDead = false;
}

// Runs when character is dead
void AEnemyCharacter::IsDeadTrue()
{
	if (DeathAnimation != NULL)
	{
		// Play death animation
		GetMesh()->PlayAnimation(DeathAnimation, false);
		
		// Ignore collisions so that don't have incorrect collisions from dead enemy
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

		// Ignore collisions with arrows
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		// Detatch AI from enemy
		DetachFromControllerPendingDestroy();

		// Set a timer so that after this time the model is deleted
		FTimerHandle DestroyTimer;
		GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemyCharacter::DestoryTimerExpired, 5.0f, false);
	}
}

// Run when destroy timer expires
void AEnemyCharacter::DestoryTimerExpired()
{
	// Get all actors attached to this actor
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	int NumAttachedActors = AttachedActors.Num();
	
	// Loop over all attached actors & delete them
	for (int i = 0; i < NumAttachedActors; i++)
	{
		AttachedActors[i]->Destroy();
	}

	// Delete self
	Destroy();
}

void AEnemyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && Cast<APlayerCharacter>(OtherActor))
	{
		IsAttacking = true;
	}
	return;
}

void AEnemyCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp && Cast<APlayerCharacter>(OtherActor))
	{
		IsAttacking = false;
	}
	return;
}