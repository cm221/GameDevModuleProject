// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDevGroupModule.h"
#include "StaticArrow.h"


// Sets default values
AStaticArrow::AStaticArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStaticArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStaticArrow::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AStaticArrow::CallOnSpawn(FVector SpawnLocationIn)
{
	SpawnLocation = SpawnLocationIn;
	OnSpawn();
}

void AStaticArrow::PendingDestroy()
{
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AStaticArrow::DestroyTimerExpired, 3.0f, false);
}

void AStaticArrow::DestroyTimerExpired()
{
	Destroy();
}

