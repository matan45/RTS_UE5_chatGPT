// Fill out your copyright notice in the Description page of Project Settings.


#include "BuilderUnit.h"

// Sets default values
ABuilderUnit::ABuilderUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildProgress = 0.0f;
	BuildTime = 5.0f;  // Default build time of 5 seconds
	bIsBuilding = false;

}



// Called when the game starts or when spawned
void ABuilderUnit::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ABuilderUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

