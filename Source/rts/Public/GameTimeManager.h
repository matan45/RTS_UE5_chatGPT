// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTimeManager.generated.h"

UCLASS()
class RTS_API AGameTimeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameTimeManager();

	bool GetPaused() { return bIsPaused; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Game time properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Time")
	int32 TotalMiniSecondsPassed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Time")
	int32 TotalSecondsPassed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Time")
	int32 MinutesPassed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Time")
	int32 HoursPassed;

	// Game time control
	UFUNCTION(BlueprintCallable, Category = "Game Time")
	void PauseTime();

	UFUNCTION(BlueprintCallable, Category = "Game Time")
	void ResumeTime();

	UFUNCTION(BlueprintCallable, Category = "Game Time")
	void SetTimeSpeed(float NewTimeSpeed);

private:
	void UpdateGameTime(float DeltaTime);

	bool bIsPaused;
	float TimeSpeed;

};
