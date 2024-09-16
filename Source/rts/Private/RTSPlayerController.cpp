#include "RTSPlayerController.h"

#include <Kismet/GameplayStatics.h>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RTSGameMode.h"
#include "RTSHUD.h"
#include "Building.h"
#include "MiniMapWidget.h"
#include "GameTimeManager.h"
#include "MyUtility.h"
#include "BuilderUnit.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"


ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Create the Spring Arm Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->TargetArmLength = 1500.0f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	// Create the Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

}


void ARTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RTShud = Cast<ARTSHUD>(UGameplayStatics::GetActorOfClass(GetWorld(), ARTSHUD::StaticClass()));

	if (!RTShud) {
		UE_LOG(LogTemp, Warning, TEXT("failed to find RTShud object"));
	}
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();

			if (InputMapping)
			{
				Subsystem->AddMappingContext(InputMapping, 1);
			}
		}

		if (IA_MoveForward)
		{
			EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ARTSPlayerController::MoveCameraForward);
		}
		if (IA_MoveRight)
		{
			EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ARTSPlayerController::MoveCameraRight);
		}
		if (IA_Zoom)
		{
			EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ARTSPlayerController::ZoomCamera);
		}
		if (IA_Select)
		{
			EnhancedInputComponent->BindAction(IA_Select, ETriggerEvent::Triggered, this, &ARTSPlayerController::Select);
		}
		if (IA_Cancel)
		{
			EnhancedInputComponent->BindAction(IA_Cancel, ETriggerEvent::Triggered, this, &ARTSPlayerController::Cancel);
		}
	}

}

void ARTSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSpringArmComponentLoction(DeltaTime);
	UpdateMiniMapPlayerIcon();

	if (Building) {
		if (Building->IsPreviewBuildingMesh()) {
			UpdateBuildingPreview();
		}
	}

}

void ARTSPlayerController::MoveCameraForward(const FInputActionValue& Value)
{
	CameraMovementInput.Y = Value.Get<float>();
}

void ARTSPlayerController::MoveCameraRight(const FInputActionValue& Value)
{
	CameraMovementInput.X = Value.Get<float>();
}

void ARTSPlayerController::ZoomCamera(const FInputActionValue& Value)
{
	CameraZoomInput = Value.Get<float>();
}


void ARTSPlayerController::Select(const FInputActionValue& Value)
{
	
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);

	if (HitResult.bBlockingHit) {
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());

		if (ABuilderUnit* Unit = Cast<ABuilderUnit>(HitActor)) {
			UE_LOG(LogTemp, Warning, TEXT("Selected BuilderUnit: %s"), *Unit->GetName());
			BuilderUnit = Unit;

			if (BuilderUnit) {
				BuilderUnit->DisplayUI(true);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("BuilderUnit is nullptr after casting."));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Hit actor is not a BuilderUnit."));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No hit detected."));
	}
}


void ARTSPlayerController::Cancel(const FInputActionValue& Value)
{
	if (Building) {
		Building->Destroy();
		Building = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Cancel action Building"));
	}
	if (BuilderUnit) {
		BuilderUnit->DisplayUI(false);
		BuilderUnit = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Cancel action BuilderUnit"));
	}
}

void ARTSPlayerController::StartPreviewBuildingSelected(ABuilding* UIBuilding)
{
	if (UIBuilding) {
		Building = UIBuilding;
	}
}


void ARTSPlayerController::UpdateSpringArmComponentLoction(float dt)
{
	if (SpringArmComponent)
	{
		// Apply the combined movement input and direction
		FVector NewLocation = SpringArmComponent->GetComponentLocation();
		NewLocation += FVector(CameraMovementInput.Y + CameraMovementDirection.Y, CameraMovementInput.X + CameraMovementDirection.X, 0) * CameraSpeed * dt;
		SpringArmComponent->SetWorldLocation(NewLocation);

		// Handle camera zoom
		float NewArmLength = SpringArmComponent->TargetArmLength + CameraZoomInput * CameraZoomSpeed * dt;
		SpringArmComponent->TargetArmLength = FMath::Clamp(NewArmLength, MinZoom, MaxZoom);

		// Reset inputs after processing
		CameraMovementInput = FVector2D::ZeroVector;
		CameraZoomInput = 0.0f;
	}
}

void ARTSPlayerController::UpdateMiniMapPlayerIcon()
{
	if (RTShud)
	{
		if (UMiniMapWidget* MiniMapWidget = RTShud->GetMiniMapWidget()) {

			// Get the player's world position
			FVector PlayerWorldLocation = GetPawn()->GetActorLocation();
			//TODO refactor mini map class for map to add and remove and update images dynamic
			// move ConvertWorldToMiniMapCoordinates to mini map class
			// Convert the world position to mini-map coordinates (implement your own logic)
			FVector2D MiniMapPosition = ConvertWorldToMiniMapCoordinates(PlayerWorldLocation);

			// Call the UpdatePlayerIconPosition function in the widget to update the player icon's position
			MiniMapWidget->UpdatePlayerIconPosition(MiniMapPosition);
		}

	}
}

//todo move it to the mini map class
FVector2D ARTSPlayerController::ConvertWorldToMiniMapCoordinates(FVector WorldLocation)
{
	// Define the bounds of the world that the mini-map covers
	FVector2D MiniMapSize = FVector2D(512.0f, 512.0f);  // Example size of the mini-map widget
	FVector2D WorldMin = FVector2D(-30000.0f, -30000.0f);  // Min world bounds covered by the mini-map
	FVector2D WorldMax = FVector2D(30000.0f, 30000.0f);    // Max world bounds covered by the mini-map

	// Map the world coordinates to mini-map coordinates
	float X = (WorldLocation.X - WorldMin.X) / (WorldMax.X - WorldMin.X) * MiniMapSize.X;
	float Y = (WorldLocation.Y - WorldMin.Y) / (WorldMax.Y - WorldMin.Y) * MiniMapSize.Y;

	return FVector2D(X, Y);
}

void ARTSPlayerController::UpdateBuildingPreview()
{
	if (Building)
	{
		Building->SetActorEnableCollision(false); // Disable collision for the preview
		FVector HitLocation;
		if (GetMouseHitLocation(HitLocation))
		{
			float TerrainHeight;
			if (GetTerrainHeightAtLocation(HitLocation, TerrainHeight)) {
				// Move the preview to the mouse hit location
				FVector TargetLocation = FVector(HitLocation.X, HitLocation.Y, TerrainHeight);

				// Smoothly move the preview towards the target location
				FVector CurrentLocation = Building->GetActorLocation();
				FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), 10.0f); // Adjust interpolation speed

				Building->SetActorLocation(NewLocation);

				// Apply rotation if you've implemented rotation functionality (current yaw rotation)
				//FRotator NewRotation = FRotator(0.0f, CurrentYawRotation, 0.0f); // Assuming you store yaw
				//Building->SetActorRotation(NewRotation);

				 // Check if the placement is valid
/*
// 				if (CanPlaceBuildingAtLocation(NewLocation, BuildingExtents))
// 				{
// 					Building->SetValidPlacement(true); // Example: Set material to green
// 				}
// 				else
// 				{
// 					Building->SetValidPlacement(false); // Example: Set material to red
// 				}
*/
			}
		}
	}
}

bool ARTSPlayerController::GetMouseHitLocation(FVector& OutHitLocation)
{
	FHitResult HitResult;
	// Get the current mouse cursor position on the screen
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D MousePosition;
	if (GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		// Convert the screen position to a world direction
		FVector WorldLocation, WorldDirection;
		DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

		// Perform a line trace (raycast) from the camera to the world
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f);  // Large enough distance to hit something in the world

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
		{
			OutHitLocation = HitResult.Location;
			return true;
		}
	}

	return false;
}

bool ARTSPlayerController::GetTerrainHeightAtLocation(const FVector& InLocation, float& OutTerrainHeight)
{
	// Define a starting point above the terrain and an endpoint far below the terrain
	FVector StartLocation = FVector(InLocation.X, InLocation.Y, InLocation.Z + 1000.0f); // 1000 units above the terrain
	FVector EndLocation = FVector(InLocation.X, InLocation.Y, InLocation.Z - 10000.0f); // 10,000 units below the terrain

	// Perform a line trace downwards to find the terrain
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Building); // Ignore the preview actor

	// Perform the line trace (use ECC_Visibility or ECC_WorldStatic for terrain/ground)
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		// If we hit something (like the terrain), retrieve the Z position of the hit
		OutTerrainHeight = HitResult.Location.Z;

		// Optionally draw debug lines to visualize the trace (for debugging)
		// DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Green, false, 1.0f, 0, 5.0f);

		return true; // We hit the terrain, so return true
	}

	return false; // We didn't hit any valid terrain
}

bool ARTSPlayerController::IsTerrainFlat(FVector BuildingLocation, FVector BuildingExtents, float Tolerance)
{
	FVector Corner1 = BuildingLocation + FVector(BuildingExtents.X, BuildingExtents.Y, 0);
	FVector Corner2 = BuildingLocation + FVector(BuildingExtents.X, -BuildingExtents.Y, 0);
	FVector Corner3 = BuildingLocation + FVector(-BuildingExtents.X, BuildingExtents.Y, 0);
	FVector Corner4 = BuildingLocation + FVector(-BuildingExtents.X, -BuildingExtents.Y, 0);

	// Perform a line trace at each corner
	float Height1, Height2, Height3, Height4;
	if (GetTerrainHeightAtLocation(Corner1, Height1) &&
		GetTerrainHeightAtLocation(Corner2, Height2) &&
		GetTerrainHeightAtLocation(Corner3, Height3) &&
		GetTerrainHeightAtLocation(Corner4, Height4))
	{
		// Check if the height difference between any corners is within the tolerance
		float MinHeight = UMyUtility::Min4(Height1, Height2, Height3, Height4);
		float MaxHeight = UMyUtility::Max4(Height1, Height2, Height3, Height4);

		return FMath::Abs(MaxHeight - MinHeight) <= Tolerance;
	}

	return false; // If we couldn't get terrain height at some point
}

bool ARTSPlayerController::CanPlaceBuildingAtLocation(FVector BuildingLocation, FVector BuildingExtents)
{
	// Check if the terrain is flat enough
	bool bIsTerrainFlat = IsTerrainFlat(BuildingLocation, BuildingExtents, 10.0f); // 10.0f tolerance for slope

	// Check if there are no other buildings or units in the way
	bool bIsLocationFree = IsLocationFreeOfObstacles(BuildingLocation, BuildingExtents);

	// Return true only if both conditions are met
	return bIsTerrainFlat && bIsLocationFree;
}

bool ARTSPlayerController::IsLocationFreeOfObstacles(FVector BuildingLocation, FVector BuildingExtents)
{
	// Define the box extents for the building
	FCollisionShape BoxCollision = FCollisionShape::MakeBox(BuildingExtents);

	// Perform a box sweep to detect overlapping actors
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Building); // Ignore the preview itself

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		BuildingLocation,
		BuildingLocation,
		FQuat::Identity, // No rotation, adjust if needed
		ECC_WorldStatic, // Channel to detect static objects (like other buildings)
		BoxCollision,
		QueryParams
	);

	return !bHit; // Return true if no hit (i.e., no obstacles detected)
}

