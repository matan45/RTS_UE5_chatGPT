// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Camera/CameraActor.h"

ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ARTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Find and attach the camera actor
	CameraActor = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass());
	// Check if CameraActor was successfully spawned
	if (CameraActor)
	{
		// Set this CameraActor as the view target
		SetViewTarget(CameraActor);
	}
	else
	{
		// Handle the case where the CameraActor could not be spawned
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn CameraActor"));
	}
	
}

void ARTSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CameraActor)
	{
		FVector NewLocation = CameraActor->GetActorLocation();

		// Apply camera movement
		NewLocation += FVector(CameraMovementInput.Y, CameraMovementInput.X, 0) * CameraSpeed * DeltaTime;

		// Apply zoom
		float NewZoom = CameraActor->GetActorLocation().Z + CameraZoomInput * CameraZoomSpeed * DeltaTime;
		NewZoom = FMath::Clamp(NewZoom, MinZoom, MaxZoom);
		NewLocation.Z = NewZoom;

		CameraActor->SetActorLocation(NewLocation);

		// Reset input values
		CameraMovementInput = FVector2D::ZeroVector;
		CameraZoomInput = 0.0f;
	}
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ARTSPlayerController::MoveCameraForward);
	InputComponent->BindAxis("MoveRight", this, &ARTSPlayerController::MoveCameraRight);
	InputComponent->BindAxis("Zoom", this, &ARTSPlayerController::ZoomCamera);
}

void ARTSPlayerController::MoveCameraForward(float Value)
{
	CameraMovementInput.X = Value;
}

void ARTSPlayerController::MoveCameraRight(float Value)
{
	CameraMovementInput.Y = Value;
}

void ARTSPlayerController::ZoomCamera(float Value)
{
	CameraZoomInput = Value;
}