// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"  // For static mesh example

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(RootComponent);
	UStaticMesh* MeshBuilding = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/StaticMeshes/Shape_Torus.Shape_Torus'"));

	if (BuildingMesh)
	{
		BuildingMesh->SetStaticMesh(MeshBuilding);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find MeshBuilding."));
	}

	PreviewBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewBuildingMesh"));
	PreviewBuildingMesh->SetupAttachment(RootComponent);
	UStaticMesh* MeshPreviewBuilding = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/StaticMeshes/Shape_QuadPyramid.Shape_QuadPyramid'"));

	if (MeshPreviewBuilding)
	{
		PreviewBuildingMesh->SetStaticMesh(MeshPreviewBuilding);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find MeshPreviewBuilding."));
	}

	StartBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StartBuildingMesh"));
	StartBuildingMesh->SetupAttachment(RootComponent);
	UStaticMesh* MeshStartBuilding = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/StaticMeshes/Shape_Sphere.Shape_Sphere'"));

	if (MeshStartBuilding)
	{
		StartBuildingMesh->SetStaticMesh(MeshStartBuilding);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find MeshStartBuilding."));
	}
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool ABuilding::IsPreviewBuildingMesh() const
{
	if (PreviewBuildingMesh) {
		return PreviewBuildingMesh->IsVisible();
	}
	UE_LOG(LogTemp, Error, TEXT("PreviewBuildingMesh is null"));
	return false;
}

bool ABuilding::IsBuildingMesh() const
{
	if (BuildingMesh) {
		return BuildingMesh->IsVisible();
	}
	UE_LOG(LogTemp, Error, TEXT("BuildingMesh is null"));
	
	return false;
}

bool ABuilding::IsStartBuildingMesh() const
{
	if (StartBuildingMesh) {
		return StartBuildingMesh->IsVisible();
	}
	UE_LOG(LogTemp, Error, TEXT("StartBuildingMesh is null"));
	
	return false;
}

void ABuilding::SetPreviewBuildingMesh(bool Visible)
{
	if (PreviewBuildingMesh) {
		PreviewBuildingMesh->SetVisibility(Visible);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PreviewBuildingMesh is null"));
	}
	
}

void ABuilding::SetBuildingMesh(bool Visible)
{
	if (BuildingMesh) {
		BuildingMesh->SetVisibility(Visible);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("BuildingMesh is null"));
	}
}

void ABuilding::SetStartBuildingMesh(bool Visible)
{
	if (StartBuildingMesh) {
		StartBuildingMesh->SetVisibility(Visible);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("StartBuildingMesh is null"));
	}
}


