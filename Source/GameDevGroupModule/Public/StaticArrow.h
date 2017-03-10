// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "StaticArrow.generated.h"

UCLASS()
class GAMEDEVGROUPMODULE_API AStaticArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticArrow();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// To call blueprint function
	void CallOnSpawn(FVector);

	UPROPERTY(BlueprintReadOnly, Category = "Spawn")
	FVector SpawnLocation;

	// Called when spawned to start timer on how long static arrow lasts
	void PendingDestroy();

protected:
	// Blueprint function called when object is hit
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawn")
	void OnSpawn();

private:
	void DestroyTimerExpired();

	
	
};
