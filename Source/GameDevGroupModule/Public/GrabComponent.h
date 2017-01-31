// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "GrabComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEDEVGROUPMODULE_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	// Value for the reach of the player in cm
	UPROPERTY(EditDefaultsOnly, Category = "Grab Setup")
	float PlayerReach = 180.0;

	// Disables roation of the object due to phsyics when set to false
	UPROPERTY(EditDefaultsOnly, Category = "Grab Setup")
	bool StopGrabbedObjectRotation = true;

	// Boolean to switch on or off drawing of debug line for players reach
	UPROPERTY(EditDefaultsOnly, Category = "Grab Setup")
	bool bDrawDebugLine = false;

	// Pointer to physics handle component
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// Pointer to the input component
	UInputComponent* InputComponent = nullptr;

	// Function called to grab object
	void Grab();

	// Function called when player releases grabbed object
	void Release();

	// Function to setup input controls
	void SetupInputComponent();

	// Function that obtains the first physics body in reach of the player
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns the initial point of the player's reach
	FVector GetReachStart() const;

	// Gets the end point of the players reach in direction they're facing
	FVector GetReachEnd() const;

	// Saves the players rotation at time when object is grabbed
	FRotator PlayerViewPointRotationAtGrab = FRotator(0.0f,0.0f,0.0f);

	// Saves objects rotation at time when it was grabbed
	FRotator ObjectRotationAtGrab = FRotator(0.0f, 0.0f, 0.0f);
	
};
