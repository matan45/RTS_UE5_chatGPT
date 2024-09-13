// Fill out your copyright notice in the Description page of Project Settings.
#include "BuilderUnit.h"
#include "BuilderUintWidget.h"

// Sets default values
ABuilderUnit::ABuilderUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}



// Called when the game starts or when spawned
void ABuilderUnit::BeginPlay()
{
	Super::BeginPlay();
	if (BuildingSelectionWidgetClass)
	{
		UBuilderUintWidget* BuildingWidget = CreateWidget<UBuilderUintWidget>(GetWorld(), BuildingSelectionWidgetClass);
		if (BuildingWidget)
		{
			BuildingWidget->AddToViewport();
		}
	}
}


// Called every frame
void ABuilderUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

