// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ARTSPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	void MoveCameraForward(float Value);
	void MoveCameraRight(float Value);
	void ZoomCamera(float Value);

private:

	FVector2D CameraMovementInput;
	float CameraZoomInput;

	UPROPERTY(EditAnywhere)
	float CameraSpeed = 2000.0f;
	UPROPERTY(EditAnywhere)
	float CameraZoomSpeed = 500.0f;
	UPROPERTY(EditAnywhere)
	float MinZoom = 300.0f;
	UPROPERTY(EditAnywhere)
	float MaxZoom = 3000.0f;

	UPROPERTY()
	ACameraActor* CameraActor;
};
