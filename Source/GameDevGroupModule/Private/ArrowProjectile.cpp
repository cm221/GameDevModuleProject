// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDevGroupModule.h"
#include "ArrowProjectile.h"
#include "ProjectileDamageType.h"
#include "StaticArrow.h"


// Sets default values
AArrowProjectile::AArrowProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup static mesh component as collision object & set as root component
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	CollisionMesh->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionMesh->OnComponentHit.AddDynamic(this, &AArrowProjectile::OnHit);
	RootComponent = CollisionMesh;

	// Stops players being able to walk over projectile.
	CollisionMesh->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionMesh->CanCharacterStepUpOn = ECB_No;

	// Setup static mesh for arrow
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Mesh"));
	ArrowMesh->SetupAttachment(RootComponent);
	ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArrowMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	// Initialising projectile movement component with default values
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = ProjectileLifeSpan;
}

void AArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	// TODO  Decide if what impulse from arrow when hitting something
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		FHitResult HitResult;

		HitWorldLocation = CollisionMesh->GetComponentLocation();
		HitWorldRotation = CollisionMesh->GetComponentRotation();

		// Setting up trace parameters
		FCollisionQueryParams TraceParameters(FName(TEXT("")), true, GetOwner());

		bool HitConfirmed = GetWorld()->SweepSingleByChannel(
			HitResult,
			HitWorldLocation,
			GetHitEndLocation(HitWorldLocation, HitWorldRotation),
			FQuat(FRotator(0,0,0)),
			ECollisionChannel::ECC_Visibility,
			FCollisionShape::MakeCapsule(0.0f, 0.0f),
			TraceParameters);

		// Check that hit was confirmed and that a bone was hit
		if (HitConfirmed && (HitResult.BoneName != "None"))
		{
			// Do damage to actor hit
			UGameplayStatics::ApplyDamage(
				HitResult.GetActor(),
				ProjectileDamage,
				GetWorld()->GetFirstPlayerController(),
				this,
				UProjectileDamageType::StaticClass());

			// try and spawn static projectile
			if (StaticProjectileBlueprint != NULL)
			{

				// spawn the projectile at the impact location
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				auto StaticArrow = GetWorld()->SpawnActor<AStaticArrow>(StaticProjectileBlueprint, HitResult.ImpactPoint, HitWorldRotation, SpawnParams);

				// Attatch StaticArrow to bone hit on object
				StaticArrow->AttachToComponent(HitResult.GetComponent(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), HitResult.BoneName);
				StaticArrow->CallOnSpawn(HitResult.ImpactPoint);
			}
			

			Destroy();
		}
		else if (HitResult.Actor != NULL)
		{
			// Enemy has been hit but arrow cant attach so just delete the arrow to hide miss
			if (HitResult.GetActor()->ActorHasTag("Enemy"))
			{
				Destroy();
			}

			// If it wasn't an enemy that was hit the spawn a static arrow at the impact location of the trace
			else
			{
				// Static arrow spawn parameters
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;

				// Spawn static arrow at the impact point of the trace
				ProjectileMovement->StopMovementImmediately();
				auto StaticArrow = GetWorld()->SpawnActor<AStaticArrow>(StaticProjectileBlueprint, HitResult.ImpactPoint, HitWorldRotation, SpawnParams);
				StaticArrow->PendingDestroy();
				Destroy();
			}	
		}
		// No actors were hit from the trace, so delete arrow
		else
		{
			Destroy();
		}
	}

}

// Called when the game starts or when spawned
void AArrowProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AArrowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// End location of linetrace from hit location
FVector AArrowProjectile::GetHitEndLocation(FVector HitWorldLocation, FRotator HitWorldRotation) const
{
	// Position of the end of the players reach using the PlayerReach varibale to determine reach length
	FVector LineTraceReach = HitWorldLocation + HitWorldRotation.Vector() * HitReach;

	return LineTraceReach;
}
