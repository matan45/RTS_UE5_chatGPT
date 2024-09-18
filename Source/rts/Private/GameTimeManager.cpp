// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimeManager.h"

// Sets default values
AGameTimeManager::AGameTimeManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize time variables
	TotalTimePassedSeconds = 0;
	SecondsPassed = 0;
	MinutesPassed = 0;
	HoursPassed = 0;

	// Time management defaults
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
	// Increment total time in seconds
	TotalTimePassedSeconds += DeltaTime;

	// Calculate total seconds, minutes, and hours
	int32 TotalSeconds = static_cast<int32>(TotalTimePassedSeconds);
	SecondsPassed = TotalSeconds % 60;
	MinutesPassed = (TotalSeconds / 60) % 60;
	int32 NewHoursPassed = TotalSeconds / 3600;

	// Check if a new hour has passed
	if (NewHoursPassed > HoursPassed)
	{
		HoursPassed = NewHoursPassed;
	}

	// Broadcast time updates
	NotifyTimeUpdate();
}


void AGameTimeManager::NotifyTimeUpdate()
{
	OnTimeUpdated.Broadcast(HoursPassed, MinutesPassed, SecondsPassed);
}

// Pauses the game time
void AGameTimeManager::PauseTime()
{
	bIsPaused = true;
	UE_LOG(LogTemp, Log, TEXT("Game time paused."));
}

// Resumes the game time
void AGameTimeManager::ResumeTime()
{
	bIsPaused = false;
	UE_LOG(LogTemp, Log, TEXT("Game time resumed."));
}

// Adjusts the game time speed
void AGameTimeManager::SetTimeSpeed(float NewTimeSpeed)
{
	TimeSpeed = FMath::Clamp(NewTimeSpeed, 0.1f, 10.0f); // Limit time speed to reasonable range
	UE_LOG(LogTemp, Log, TEXT("Game time speed set to: %f"), TimeSpeed);
}