// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovement;

	// Collision component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UStaticMeshComponent* CollisionMesh;

	// When projectile hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	float ProjectileDamage = 20.0f;

protected:
	// Blueprint function called when object is hit
	UFUNCTION(BlueprintImplementableEvent, Category = "Score")
	void ObjectHit();

private:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float ProjectileLifeSpan = 3.0f;
};
