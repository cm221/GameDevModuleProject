// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AArrowProjectile;
class AInteractionObject;

UCLASS()
class GAMEDEVGROUPMODULE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// First person camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCameraComponent;

	// Physics Handle Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PhysicsHandle")
	class UPhysicsHandleComponent* PhysicsHandleComponent;

	// Crossbow static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* CrossBowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* SwordArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* SwordMesh;

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	// Projectile to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AArrowProjectile> ProjectileBlueprint;

	// Sound played when projectile is fired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class USoundBase* FireSound;

	// Get Interaction object in reach
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AInteractionObject* GetInteractionObjectInReach();

	// Boolean that is true if there's an interaction object in reach
	UPROPERTY(BlueprintReadOnly)
	bool ConfirmedInteractionObjectInReach = false;

	// boolean checking that is true if player is attacking
	UPROPERTY(BlueprintReadOnly, Category = "Attacking")
	bool IsAttacking = false;

	// Boolean variable that indicates if enemy is dead or alive
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
	bool isDead = false;

	// Health property
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
	int32 CurrentHealth;

	// Boolean that indicates if the attack animation is running
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attacking")
	bool AttackAnimRunning = false;

	// Boolean that indicates if damage has been done this attack
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attacking")
	bool DamageDoneThisAttack = false;

	// Returns initial health value set
	UFUNCTION(BlueprintCallable, Category = "Health")
	int32 GetInitalHealth() const;

	// Overlap event with sword
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Fires a projectile
	void Fire();

	// Attacks with sword
	void SwordAttack();

	// Attacks with sword
	void StopSwordAttack();

	// Forward & back movement
	void MoveForward(float Val);

	// Left & right movement
	void MoveRight(float Val);

	// Look right & left rate for controller input
	void LookRightAtRate(float LookRate);

	// Look up & down rate for controller input
	void LookUpAtRate(float LookRate);

	// Start crouch when crouch pressed
	void StartCrouch();

	// Uncrouches when crouch button is released
	void StopCrouch();

	// Start running when button is pressed
	void StartRun();

	// Stop running when button is released
	void StopRun();

	// Initial speed of walking set by movement component, saved so that can set back to this post running
	float InitialWalkSpeed;

	// Start location of player reach
	FVector GetReachStart() const;

	// End location of player reach
	FVector GetReachEnd() const;

	// Checks if the player is dead 
	virtual void IsDeadCheck();

	// Causes player to take damage when hit
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
	class AController * EventInstigator, AActor * DamageCauser) override;

	// Blueprint function called enemy is hit with sword
	UFUNCTION(BlueprintImplementableEvent, Category = "Score")
	void SwordDamageConfirmed(FVector BloodSpawnLocation);

private:
	// Value for the reach of the player in cm
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float PlayerReach = 180.0;

	// Time from hit confirmed to spawning blood particles in seconds
	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	float TimeToSpawnBlood = 0.2;
	
	// Initial health value
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	int32 InitialHealth = 100;	

	// Damage sword can do
	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	float SwordDamage = 20.0f;

	// Function called after time to activate blood particles
	void ActivateBloodParticles();

	// Actor hit by sword
	AActor* ActorHitBySword = NULL;
};
