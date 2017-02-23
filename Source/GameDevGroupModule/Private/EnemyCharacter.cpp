// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDevGroupModule.h"
#include "EnemyCharacter.h"
#include "ProjectileDamageType.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Check if enemy is dead
void AEnemyCharacter::IsDeadCheck()
{
	if (CurrentHealth < 0)
		isDead = true;
	else
		isDead = false;
}

// Called by the engine when damage is dealt
float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
class AController * EventInstigator, AActor * DamageCauser)
{

	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;

	IsDeadCheck();

	UE_LOG(LogTemp, Warning, TEXT("Damage Done: %i"), DamageToApply);

	if (Cast<UProjectileDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Test worked"));
	}

	return DamageToApply;
}