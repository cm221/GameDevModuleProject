// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class AStaticArrow;

UCLASS()
class GAMEDEVGROUPMODULE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Projectile movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	// Collision component
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	class UStaticMeshComponent* CollisionMesh;

	// Arrow mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	class UStaticMeshComponent* ArrowMesh;

	// When projectile hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Static projectile to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AStaticArrow> StaticProjectileBlueprint;

protected:
	// Blueprint function called when object is hit
	UFUNCTION(BlueprintImplementableEvent, Category = "Score")
	void ObjectHit();

private:
	// Lifespan of projectile in air before being destroyed
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileLifeSpan = 3.0f;

	// Damage projectile can do
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileDamage = 20.0f;

	// End of capsule trace from hit event location
	FVector GetHitEndLocation(FVector, FRotator) const;

	// World location of where arrow hits object
	FVector HitWorldLocation;
	
	// Rotation of arrow in world when hits object
	FRotator HitWorldRotation;

	// Reach of the line trace when arrow hits object
	float HitReach = 200.0f;
};
