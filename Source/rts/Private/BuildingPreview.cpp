// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPreview.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

// Sets default values
ABuildingPreview::ABuildingPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	RootComponent = BuildingMesh;
}

// Called when the game starts or when spawned
void ABuildingPreview::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildingPreview::FinishPlacingBuilding()
{
	// Finalize the building process (e.g., turn off preview mode)
	Destroy();
}

void ABuildingPreview::SetIsValidLocation(bool bIsValid)
{
	if (bIsValid && ValidLocationMaterial && InvalidLocationMaterial)
	{
		BuildingMesh->SetMaterial(0, ValidLocationMaterial);
	}
	else
	{
		BuildingMesh->SetMaterial(0, InvalidLocationMaterial);
	}
}

// Called every frame
void ABuildingPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

