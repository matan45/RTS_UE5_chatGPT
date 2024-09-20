// Fill out your copyright notice in the Description page of Project Settings.
#include "BuilderUnit.h"
#include "BuilderUintWidget.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ABuilderUnit::ABuilderUnit()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);  // Use the custom channel you created
}

// Called when the game starts or when spawned
void ABuilderUnit::BeginPlay()
{
	Super::BeginPlay();
	if (BuildingSelectionWidgetClass)
	{
		BuildingWidget = CreateWidget<UBuilderUintWidget>(GetWorld(), BuildingSelectionWidgetClass);

		if (BuildingWidget)
		{
			UE_LOG(LogTemp, Log, TEXT("Building widget created successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create building widget."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BuildingSelectionWidgetClass is not set."));
	}
}


void ABuilderUnit::DisplayUI(bool bShowUI)
{
	if (bShowUI)
	{
		if (BuildingWidget)
		{
			BuildingWidget->AddToViewport();
		}
	}
	else if (BuildingWidget)
	{
		BuildingWidget->RemoveFromParent();
	}
}

// Called every frame
void ABuilderUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

