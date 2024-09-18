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

// Methods to toggle visibility of building components
bool ABuilding::IsPreviewBuildingMesh() const
{
	return PreviewBuildingMesh ? PreviewBuildingMesh->IsVisible() : false;
}

bool ABuilding::IsBuildingMesh() const
{
	return BuildingMesh ? BuildingMesh->IsVisible() : false;
}

bool ABuilding::IsStartBuildingMesh() const
{
	return StartBuildingMesh ? StartBuildingMesh->IsVisible() : false;
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

