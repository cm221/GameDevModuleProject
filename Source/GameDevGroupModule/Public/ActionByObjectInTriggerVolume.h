// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ActionByObjectInTriggerVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventBroadcast);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEDEVGROUPMODULE_API UActionByObjectInTriggerVolume : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionByObjectInTriggerVolume();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Event called in blueprint when object is found in volume
	UPROPERTY(BlueprintAssignable)
	FEventBroadcast ObjectFound;

	// Event called in blueprint when object is not in volume
	UPROPERTY(BlueprintAssignable)
	FEventBroadcast ObjectNotFound;


private:
	// Trigger volume assigned to owner of this component in which the desired object is looked for
	UPROPERTY(EditAnywhere)
	ATriggerVolume* ObjectCheckVolume = nullptr;

	// Actor that activates this when in trigger volume
	UPROPERTY(EditAnywhere)
	AActor* ActorThatActivates = nullptr;
};
