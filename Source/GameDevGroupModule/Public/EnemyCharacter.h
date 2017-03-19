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

	// Volume for attacking player in
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	class UBoxComponent* AttackVolume;

	// Causes enemy to take damage when hit
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
	class AController * EventInstigator, AActor * DamageCauser) override;

	// Overlap event with attack volume
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Overlap end with attack volume
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Bool variable denoting if enemy is attacking
	UPROPERTY(BlueprintReadOnly, Category = "Attacking")
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimSequence* DeathAnimation;

	// Damage enemy can do
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float AttackDamage = 20.0f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	int32 InitialHealth = 100;

	void DestoryTimerExpired();

	// Boolean variable that indicates if enemy is dead or alive
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float isDead = false;

	// Health property
	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth;

	// Checks if the enemy is dead 
	void IsDeadCheck();

	// Called when enemy is dead
	void IsDeadTrue();
	
};
