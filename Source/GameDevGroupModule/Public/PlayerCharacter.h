// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AProjectile;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FirstPersonCameraComponent;

	// Physics Handle Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PhysicsHandle)
	class UPhysicsHandleComponent* PhysicsHandleComponent;

	// Crossbow static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class UStaticMeshComponent* CrossBowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* SwordArmMesh;

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	// Projectile to spawn
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	// Sound played when projectile is fired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AInteractionObject* GetInteractionObjectInReach();

	UPROPERTY(BlueprintReadOnly)
	bool ConfirmedInteractionObjectInReach = false;

	UPROPERTY(BlueprintReadOnly, Category = "Attacking")
	bool IsAttacking = false;

	// Boolean variable that indicates if enemy is dead or alive
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
	float isDead = false;

	// Health property
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
	int32 CurrentHealth;

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

private:
	// Value for the reach of the player in cm
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float PlayerReach = 180.0;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	int32 InitialHealth = 100;	
};
