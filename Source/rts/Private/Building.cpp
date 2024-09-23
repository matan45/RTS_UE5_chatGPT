// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"


// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Root scene component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Main building mesh (completed)
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(RootComponent);

	// Preview building mesh (used before placement)
	PreviewBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewBuildingMesh"));
	PreviewBuildingMesh->SetupAttachment(RootComponent);

	// Start building mesh (used during construction)
	StartBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StartBuildingMesh"));
	StartBuildingMesh->SetupAttachment(RootComponent);

	// Initialize building state
	CurrentState = EBuildingState::Preview;

	// In constructor, initialize the range
	FogOfWarRange = CreateDefaultSubobject<USphereComponent>(TEXT("FogOfWarRange"));
	FogOfWarRange->SetupAttachment(RootComponent);
	FogOfWarRange->SetSphereRadius(ViewRage); // Adjust this as needed
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Any custom building logic can go here
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	// Initially set the building in preview mode
	SetBuildingState(EBuildingState::Preview);
}

void ABuilding::SetPreviewBuildingMesh(bool Visible)
{
	if (PreviewBuildingMesh)
	{
		PreviewBuildingMesh->SetVisibility(Visible);
	}
}

void ABuilding::SetBuildingMesh(bool Visible)
{
	if (BuildingMesh)
	{
		BuildingMesh->SetVisibility(Visible);
	}
}

void ABuilding::SetStartBuildingMesh(bool Visible)
{
	if (StartBuildingMesh)
	{
		StartBuildingMesh->SetVisibility(Visible);
	}
}

// Method to change building state and update mesh visibility accordingly
void ABuilding::SetBuildingState(EBuildingState NewState)
{
	CurrentState = NewState;

	switch (NewState)
	{
	case EBuildingState::Preview:
		SetPreviewBuildingMesh(true);
		SetBuildingMesh(false);
		SetStartBuildingMesh(false);
		break;

	case EBuildingState::UnderConstruction:
		SetPreviewBuildingMesh(false);
		SetBuildingMesh(false);
		SetStartBuildingMesh(true);
		break;

	case EBuildingState::Completed:
		SetPreviewBuildingMesh(false);
		SetBuildingMesh(true);
		SetStartBuildingMesh(false);
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown building state."));
		break;
	}
}

// Helper function to change the building's material depending on placement validity
void ABuilding::UpdatePlacementMaterial(bool bIsValid)
{
	if (PreviewBuildingMesh)
	{
		if (bIsValid && ValidLocationMaterial)
		{
			PreviewBuildingMesh->SetMaterial(0, ValidLocationMaterial);
		}
		else if (!bIsValid && InvalidLocationMaterial)
		{
			PreviewBuildingMesh->SetMaterial(0, InvalidLocationMaterial);
		}
	}
}

float ABuilding::TakeDamage(
	float DamageAmount,
	const FDamageEvent& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	// You can add custom logic here for how the building takes damage
	// For example, reduce health based on DamageAmount
	BuildingHealth -= DamageAmount;

	// Check if the building should be destroyed
	if (BuildingHealth <= 0.0f)
	{
		// Optionally destroy the building or handle destruction logic
		Destroy();
	}

	// Optionally call the base class implementation if needed
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABuilding::Construction(float DeltaTime)
{
	if (CurrentState == EBuildingState::UnderConstruction)
	{
		ElapsedConstructionTime += DeltaTime;
		float Progress = ElapsedConstructionTime / ConstructionTime;
		//show contraction progress bar
		// Example: Update your progress bar in UI (bind or call a Blueprint function)
		if (Progress >= 1.0f)
		{
			SetBuildingState(EBuildingState::Completed);
			//and notify user
		}
	}
}

// Add a unit to the queue and start training if not already
void ABuilding::AddUnitToQueue(TSubclassOf<AActor> UnitClass)
{
	if (UnitClass)
	{
		TrainingQueue.Add(UnitClass);
		UE_LOG(LogTemp, Log, TEXT("Unit added to training queue"));

		// If this is the only unit in the queue, start training immediately
		if (TrainingQueue.Num() == 1)
		{
			TrainNextUnit();
		}
	}
}

// Train the next unit in the queue
void ABuilding::TrainNextUnit()
{
	if (TrainingQueue.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Training next unit..."));

		// Set a timer to simulate unit training over time
		GetWorld()->GetTimerManager().SetTimer(TrainingTimerHandle, this, &ABuilding::TrainNextUnit, UnitTrainingTime, false);

		// Get the unit class at the front of the queue
		TSubclassOf<AActor> UnitClass = TrainingQueue[0];

		// Remove the unit from the queue (front of the queue)
		TrainingQueue.RemoveAt(0);

		// After training time, spawn the unit
		SpawnTrainedUnit(UnitClass);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Training queue is empty."));
	}
}

// Function to spawn the unit after training is done
void ABuilding::SpawnTrainedUnit(TSubclassOf<AActor> UnitClass)
{
	if (UnitClass)
	{
		// Example: spawn the unit near the building's location
		FVector SpawnLocation = GetActorLocation() + FVector(200.0f, 0.0f, 0.0f); // Adjust as needed
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// Spawn the unit
		GetWorld()->SpawnActor<AActor>(UnitClass, SpawnLocation, SpawnRotation);
		UE_LOG(LogTemp, Log, TEXT("Unit trained and spawned."));

		// Check if there are more units to train in the queue
		if (TrainingQueue.Num() > 0)
		{
			TrainNextUnit();  // Continue training the next unit
		}
	}
}

