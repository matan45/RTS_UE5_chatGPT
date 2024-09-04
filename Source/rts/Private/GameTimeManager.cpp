// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimeManager.h"

// Sets default values
AGameTimeManager::AGameTimeManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize time variables
	TotalSecondsPassed = 0;
	MinutesPassed = 0;
	HoursPassed = 0;

	// Initialize time control variables
	bIsPaused = false;
	TimeSpeed = 1.0f; // Normal speed
}

void AGameTimeManager::BeginPlay()
{
	Super::BeginPlay();

}

void AGameTimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsPaused)
	{
		UpdateGameTime(DeltaTime * TimeSpeed);
	}
}

void AGameTimeManager::UpdateGameTime(float DeltaTime)
{
	// Accumulate time
	TotalSecondsPassed += FMath::RoundToInt(DeltaTime)+1;

	// Calculate minutes and hours
	MinutesPassed = TotalSecondsPassed / 60;
	HoursPassed = MinutesPassed / 60;
}

void AGameTimeManager::PauseTime()
{
	bIsPaused = true;
}

void AGameTimeManager::ResumeTime()
{
	bIsPaused = false;
}

void AGameTimeManager::SetTimeSpeed(float NewTimeSpeed)
{
	TimeSpeed = NewTimeSpeed;
}