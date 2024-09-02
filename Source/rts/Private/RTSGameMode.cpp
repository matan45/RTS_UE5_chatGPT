// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGameMode.h"
#include "RTSPlayerController.h"
#include "GameTimeManager.h"
#include "RTSHUD.h"
#include "UObject/ConstructorHelpers.h"

ARTSGameMode::ARTSGameMode()
{
	PlayerControllerClass = ARTSPlayerController::StaticClass();
	HUDClass = ARTSHUD::StaticClass();
}

void ARTSGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		
		// Spawn the GameTimeManager and store the reference
		GameTimeManager = GetWorld()->SpawnActor<AGameTimeManager>(AGameTimeManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

		if (!GameTimeManager)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn GameTimeManager"));
		}
	}
}

