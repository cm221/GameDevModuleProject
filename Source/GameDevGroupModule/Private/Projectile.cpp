// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDevGroupModule.h"
#include "ProjectileDamageType.h"
#include "Projectile.h"
#include "StaticArrow.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup static mesh component as collision object & set as root component
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	CollisionMesh->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
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

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// TODO: Remove if projectile can't bounce or if dont want object to move other objects
	// Only add impulse and destroy projectile if we hit a physics
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
			FQuat(HitWorldRotation),
			ECollisionChannel::ECC_WorldStatic,
			FCollisionShape::MakeCapsule(10.0f, 10.0f),
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

				// Stop projectile movement
				ProjectileMovement->StopMovementImmediately();

				// try and spawn static projectile
				if (StaticProjectileBlueprint != NULL)
				{

					// spawn the projectile at the socket location
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = Instigator;
					auto StaticArrow = GetWorld()->SpawnActor<AStaticArrow>(StaticProjectileBlueprint, (HitResult.ImpactPoint + HitWorldRotation.Vector() * 20.f), HitWorldRotation, SpawnParams);

					// Attatch StaticArrow to bone hit on object
					StaticArrow->AttachToComponent(HitResult.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, HitResult.BoneName);
					StaticArrow->CallOnSpawn(HitResult.ImpactPoint);
				}

				Destroy();
			}
			else if (HitResult.GetActor()->ActorHasTag("Enemy"))
			{
				Destroy();
			}
			else
			{
				// spawn the projectile at the socket location
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;

				ProjectileMovement->StopMovementImmediately();
				auto StaticArrow = GetWorld()->SpawnActor<AStaticArrow>(StaticProjectileBlueprint, HitWorldLocation, HitWorldRotation, SpawnParams);
				StaticArrow->PendingDestroy();
				Destroy();
			}
	}

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// End location of linetrace from hit location
FVector AProjectile::GetHitEndLocation(FVector HitWorldLocation, FRotator HitWorldRotation) const
{
	// Position of the end of the players reach using the PlayerReach varibale to determine reach length
	FVector LineTraceReach = HitWorldLocation + HitWorldRotation.Vector() * HitReach;

	return LineTraceReach;
}
