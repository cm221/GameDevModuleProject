// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDevGroupModule.h"
#include "PlayerCharacter.h"
#include "Projectile.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set character to tick
	PrimaryActorTick.bCanEverTick = true;

	// Size of collision capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 90.0f); // 90cm half height makes the character approx 6ft

	// Set turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create & attach a First Person Camera Component to the capsule
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create crossbow static mesh & attach to capsule
	CrossBowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cross Bow Mesh"));
	CrossBowMesh->SetupAttachment(FirstPersonCameraComponent);
	CrossBowMesh->RelativeLocation = FVector(0.0f, 30.0f, -30.0f); // Position the crossbow
	CrossBowMesh->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
	CrossBowMesh->SetOnlyOwnerSee(true);
	CrossBowMesh->bCastCinematicShadow = false;
	CrossBowMesh->CastShadow = false;


	// Allows character to crouch
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Create & attach physics handle component to the capsule
	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
}

// Called every frame
void APlayerCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::StopCrouch);

	InputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::StartRun);
	InputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::StopRun);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	InputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookRightRate", this, &APlayerCharacter::LookRightAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Fire);

}

// Forward & back movement
void APlayerCharacter::MoveForward(float MoveInput)
{
	if (MoveInput != 0.0f)
	{
		// Add forward  & back movement
		AddMovementInput(GetActorForwardVector(), MoveInput);
	}
}

// Left & right movement
void APlayerCharacter::MoveRight(float MoveInput)
{
	if (MoveInput != 0.0f)
	{
		// Add left & right movement
		AddMovementInput(GetActorRightVector(), MoveInput);
	}
}

// Look right & left rate for controller input
void APlayerCharacter::LookRightAtRate(float LookRate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(LookRate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// Look up & down rate for controller input
void APlayerCharacter::LookUpAtRate(float LookRate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(LookRate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Start crouch when crouch pressed
void APlayerCharacter::StartCrouch()
{
	Crouch();
}

// Uncrouches when crouch button is released
void APlayerCharacter::StopCrouch()
{
	UnCrouch();
}

// Start running when button is pressed
void APlayerCharacter::StartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxCustomMovementSpeed;
}

// Stop running when button is released
void APlayerCharacter::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = InitialWalkSpeed;
}

void APlayerCharacter::Fire()
{
	// try and fire a projectile
	if (ProjectileBlueprint != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();

		// Spawn location is the socket at the end of the crossbow
		const FVector SpawnLocation = CrossBowMesh->GetSocketLocation(FName("ProjectilePoint"));

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<AProjectile>(ProjectileBlueprint, SpawnLocation, SpawnRotation);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

// Find the first physics type object in the players reach
FHitResult APlayerCharacter::GetInteractionActorInReach()
{
	FHitResult HitResult;

	// Setting up trace parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Linetrace in the world & return first intercepted object
	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		GetReachStart(),
		GetReachEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);

	return HitResult;
}

// Start position of the player's grab reach
FVector APlayerCharacter::GetReachStart() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Returns the position of the player's camera view
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

// End position of the player's grab reach
FVector APlayerCharacter::GetReachEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Returns the position of the player's camera view
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	// Position of the end of the players reach using the PlayerReach varibale to determine reach length
	FVector PlayerReachEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * PlayerReach;

	return PlayerReachEnd;
}