// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS(Blueprintable)
class GAMEDEVGROUPMODULE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Health property
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
	int32 CurrentHealth;

	// Boolean variable that indicates if enemy is dead or alive
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
	float isDead = false;

	// Checks if the enemy is dead 
	virtual void IsDeadCheck();

	// Causes enemy to take damage when hit
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
	class AController * EventInstigator, AActor * DamageCauser) override;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	int32 InitialHealth = 100;
	
};
