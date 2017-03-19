// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDevGroupModule.h"
#include "GrabComponent.h"


// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// Finding physics handle component on player
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	// Setting up the input component for using the grab mechanic
	SetupInputComponent();
	
}


// Called every frame
void UGrabComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Checking that have a physics handle component
	if (!ensure(PhysicsHandle)) { return; }

	// Checks if an object is grabbed
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object that is grabbed 
		PhysicsHandle->SetTargetLocation(GetReachEnd());

		// Gets the player's camera location and rotation
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			PlayerViewPointLocation,
			PlayerViewPointRotation
		);

		// Takes the rotation of the object from when it was grabbed & uses the players rotation since then to ensure that the object is always facing same way since it was grabbed
		FRotator ObjectRotation = ObjectRotationAtGrab;
		ObjectRotation.Yaw += PlayerViewPointRotation.Yaw - PlayerViewPointRotationAtGrab.Yaw;

		// Setting the rotation of the grabbed object
		PhysicsHandle->SetTargetRotation(ObjectRotation);
	}
}

// Sets up the input for the grab mechanic
void UGrabComponent::SetupInputComponent()
{
	// Searches for the input component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	
	// Check that found an input component
	if (!ensure(InputComponent)) { return; }

	// Binding grab action to grab & release function
	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabComponent::Grab);
	//InputComponent->BindAction("Grab", IE_Released, this, &UGrabComponent::Release);
}

// Called when player presses grab button
void UGrabComponent::Grab()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		Release();
	}
	else
	{
		// Line trace to find if physics object in player's reach
		FHitResult HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent();
		auto ActorHit = HitResult.GetActor();

		// Check if an actor was intercepted by line trace & has a physics handle component
		if (!ActorHit) { return; }
		if (!PhysicsHandle) { return; }

		// Finds orign of actor for location of where to grab
		FVector ActorOrigin, ActorBounds;
		ComponentToGrab->GetOwner()->GetActorBounds(true, ActorOrigin, ActorBounds);

		// Place physics handle at the objects origin
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ActorOrigin,
			StopGrabbedObjectRotation); // Determines if grabbed object can be rotated

										// Saving the players view rotation when the object was grabbed so to be able to rotate object with camera
		FVector PlayerViewPointLocation;
		PlayerViewPointRotationAtGrab;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			PlayerViewPointLocation,
			PlayerViewPointRotationAtGrab
			);

		// Saving the objects when it was grabbed
		FVector ObjectLocation;
		PhysicsHandle->GetTargetLocationAndRotation(ObjectLocation, ObjectRotationAtGrab);
	}
}

// Called when player releases grab button
void UGrabComponent::Release()
{
	// Check if have a physics handle component
	if (!PhysicsHandle) { return; }

	// Release the grabbed component
	PhysicsHandle->ReleaseComponent();
}

// Find the first physics type object in the players reach
const FHitResult UGrabComponent::GetFirstPhysicsBodyInReach()
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

	// Draw debug line if DrawDebugLine set to true in blueprints for debugging
	if (bDrawDebugLine)
	{
		DrawDebugLine(
			GetWorld(),
			GetReachStart(),
			GetReachEnd(),
			FColor(255, 0, 0),
			true, -1, 0, 10);
	}
	
	return HitResult;
}

// Start position of the player's grab reach
FVector UGrabComponent::GetReachStart() const
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
FVector UGrabComponent::GetReachEnd() const
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