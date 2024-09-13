// Fill out your copyright notice in the Description page of Project Settings.
#include "BuilderUintWidget.h"
#include "Building.h"
#include "RTSPlayerController.h"
#include "Kismet/GameplayStatics.h"


void UBuilderUintWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the button click event
	if (buildingIcon)
	{
		buildingIcon->OnClicked.AddDynamic(this, &UBuilderUintWidget::SeletedFristBuilding);
	}

	PlayerController = Cast<ARTSPlayerController>(GetOwningPlayer());
}



void UBuilderUintWidget::SeletedFristBuilding()
{


	// Assuming you have a valid ABuildingPreview class to spawn
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;

		// Replace ABuildingPreview with your actual class
		Building = GetWorld()->SpawnActor<ABuilding>(ABuilding::StaticClass(), SpawnParams);

		FVector NewLocation = FVector(33670.0f, 9640.0f, 60.0f);
		Building->SetActorLocation(NewLocation);
		Building->SetActorHiddenInGame(false);
		Building->SetBuildingMesh(true);
	}


	if (Building && PlayerController)
	{

		// Your building logic here...
		//Building->SetPreviewBuildingMesh(true);
		//Building->SetBuildingMesh(false);
		//Building->SetStartBuildingMesh(false);
		//PlayerController->StartPreviewBuildingSelected(Building);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Building or PlayerController is null in SeletedFristBuilding."));
	}
}

