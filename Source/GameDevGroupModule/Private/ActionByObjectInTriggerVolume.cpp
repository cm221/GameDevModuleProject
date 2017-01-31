// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDevGroupModule.h"
#include "ActionByObjectInTriggerVolume.h"


// Sets default values for this component's properties
UActionByObjectInTriggerVolume::UActionByObjectInTriggerVolume()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActionByObjectInTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	// Check that door is assigned a trigger volume
	if (!ObjectCheckVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing trigger volume"), *GetOwner()->GetName())
	}
	if (!ActorThatActivates)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing actor to trigger"), *GetOwner()->GetName())
	}
	
}


// Called every frame
void UActionByObjectInTriggerVolume::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (ObjectCheckVolume->IsOverlappingActor(ActorThatActivates))
	{
		ObjectFound.Broadcast();
	}
	else
	{
		ObjectNotFound.Broadcast();
	}
}

