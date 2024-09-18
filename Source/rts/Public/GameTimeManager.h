// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTimeManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTimeUpdated, int32, Hours, int32, Minutes, int32, Seconds);

UCLASS()
class RTS_API AGameTimeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameTimeManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Pauses the game time
	UFUNCTION(BlueprintCallable, Category = "Game Time")
	void PauseTime();

	// Resumes the game time
	UFUNCTION(BlueprintCallable, Category = "Game Time")
	void ResumeTime();

	// Adjusts the game time speed
	UFUNCTION(BlueprintCallable, Category = "Game Time")
	void SetTimeSpeed(float NewTimeSpeed);

	// Returns whether the game time is paused
	UFUNCTION(BlueprintCallable, Category = "Game Time")
	bool IsPaused() const { return bIsPaused; }

	// Event that fires when an hour passes
	UPROPERTY(BlueprintAssignable, Category = "Game Time")
	FOnTimeUpdated OnTimeUpdated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Tracks the total time passed
	double TotalTimePassedSeconds;

	// Game time properties for hours, minutes, and seconds
	int32 SecondsPassed;
	int32 MinutesPassed;
	int32 HoursPassed;

	// Whether the game time is paused
	bool bIsPaused;

	// The speed at which the game time progresses (1.0 is real time, 2.0 is double speed, etc.)
	float TimeSpeed;

	// Updates the game time, triggered every frame
	void UpdateGameTime(float DeltaTime);

	// Notifies when an hour passes
	void NotifyTimeUpdate();
};
