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

	// Cache player controller reference
	PlayerController = Cast<ARTSPlayerController>(GetOwningPlayer());
}



void UBuilderUintWidget::SeletedFristBuilding()
{
	if (BuildingClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		Building = GetWorld()->SpawnActor<ABuilding>(BuildingClass, SpawnParams);
	}

	if (Building && PlayerController)
	{
		// Set building to preview mode
		Building->SetBuildingState(EBuildingState::Preview);

		// Notify PlayerController to start the building placement process
		PlayerController->StartPreviewBuildingSelected(Building);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Building or PlayerController is null in SeletedFristBuilding."));
	}
}

