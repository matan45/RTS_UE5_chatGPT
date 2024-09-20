#include "RTSPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Building.h"
#include "BuilderUnit.h"
#include "MyUtility.h"
#include "RTSHUD.h"
#include "MiniMapWidget.h"

// Constructor
ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Create the Spring Arm Component for camera movement
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->TargetArmLength = 1500.0f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	// Create the Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts
void ARTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RTShud = Cast<ARTSHUD>(UGameplayStatics::GetActorOfClass(GetWorld(), ARTSHUD::StaticClass()));

	if (!RTShud) {
		UE_LOG(LogTemp, Warning, TEXT("failed to find RTShud object"));
	}

}

// Setup input actions
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

// Called every frame
void ARTSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleEdgeScrolling(DeltaTime);

	UpdatePawnLocation(DeltaTime);

	UpdateSpringArmLocation(DeltaTime);

	if (SelectedBuilding && SelectedBuilding->IsPreviewBuildingMesh())
	{
		UpdateBuildingPreview();
	}

	UpdateMinimap();
}

// Camera movement
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

void ARTSPlayerController::HandleEdgeScrolling(float DeltaTime)
{
	FVector2D MousePosition;
	FVector2D ViewportSize;

	// Get the mouse position and viewport size
	if (GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		// Edge scrolling logic for the X axis (left-right movement)
		if (MousePosition.X <= EdgeScrollThreshold) // Left edge
		{
			CameraMovementInput.X = -1;
		}
		else if (MousePosition.X >= ViewportSize.X - EdgeScrollThreshold) // Right edge
		{
			CameraMovementInput.X = 1;
		}
		else
		{
			CameraMovementInput.X = 0;
		}

		// Edge scrolling logic for the Y axis (forward-backward movement)
		if (MousePosition.Y <= EdgeScrollThreshold) // Top edge
		{
			CameraMovementInput.Y = 1;
		}
		else if (MousePosition.Y >= ViewportSize.Y - EdgeScrollThreshold) // Bottom edge
		{
			CameraMovementInput.Y = -1;
		}
		else
		{
			CameraMovementInput.Y = 0;
		}
	}
}

// Update camera movement and zoom
void ARTSPlayerController::UpdateSpringArmLocation(float DeltaTime)
{
	if (SpringArmComponent)
	{
		// Move the SpringArmComponent instead of the CameraComponent
		FVector CurrentLocation = SpringArmComponent->GetComponentLocation();
		FVector TargetLocation = CurrentLocation + FVector(CameraMovementInput.Y, CameraMovementInput.X, 0) * CameraSpeed * DeltaTime;
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 5.0f); // Smooth movement
		SpringArmComponent->SetWorldLocation(NewLocation);  // Move spring arm (camera will follow)

		// Smooth zooming
		float NewArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, SpringArmComponent->TargetArmLength + CameraZoomInput * CameraZoomSpeed, DeltaTime, 5.0f);
		SpringArmComponent->TargetArmLength = FMath::Clamp(NewArmLength, MinZoom, MaxZoom);

		// Reset inputs after processing
		CameraMovementInput = FVector2D::ZeroVector;
		CameraZoomInput = 0.0f;
	}
}


void ARTSPlayerController::UpdatePawnLocation(float DeltaTime)
{
	if (APawn* ControlledPawn = GetPawn())  // Get the pawn controlled by the player
	{
		// Calculate the movement direction based on input
		FVector CurrentLocation = ControlledPawn->GetActorLocation();
		FVector TargetLocation = CurrentLocation + FVector(CameraMovementInput.Y, CameraMovementInput.X, 0) * CameraSpeed * DeltaTime;

		// Set the new location for the pawn
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 5.0f);  // Smooth movement
		ControlledPawn->SetActorLocation(NewLocation);  // Move the pawn directly
	}
}

// Handle building selection
void ARTSPlayerController::Select(const FInputActionValue& Value)
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;

	// Ignore the player's pawn
	QueryParams.AddIgnoredActor(GetPawn());

	// Get the mouse position in screen space
	FVector2D MousePosition;
	if (GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		FVector WorldLocation, WorldDirection;

		// Convert the 2D screen position to a 3D world direction
		DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

		// Define the trace start and end points
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f); // A long trace distance

		// Perform the trace
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, QueryParams))
		{
			AActor* HitActor = HitResult.GetActor();
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());

			// Check if the hit actor is a BuilderUnit
			if (ABuilderUnit* Unit = Cast<ABuilderUnit>(HitActor))
			{
				SelectedBuilderUnit = Unit;

				if (SelectedBuilderUnit)
				{
					SelectedBuilderUnit->DisplayUI(true);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("BuilderUnit is nullptr after casting."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit actor is not a BuilderUnit."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No hit detected."));
		}
	}
}

// Cancel building or selection
void ARTSPlayerController::Cancel(const FInputActionValue& Value)
{
	if (SelectedBuilding)
	{
		SelectedBuilding->Destroy();
		SelectedBuilding = nullptr;
	}
	if (SelectedBuilderUnit) {
		SelectedBuilderUnit->Destroy();
		SelectedBuilderUnit = nullptr;
	}
}

// Start the preview building selection process
void ARTSPlayerController::StartPreviewBuildingSelected(ABuilding* UIBuilding)
{
	if (UIBuilding)
	{
		SelectedBuilding = UIBuilding;
		SelectedBuilding->SetPreviewBuildingMesh(true);
	}
}

// Update the building preview location during placement
void ARTSPlayerController::UpdateBuildingPreview()
{
	if (SelectedBuilding)
	{
		SelectedBuilding->SetActorEnableCollision(false); // Disable collision for the preview
		FVector HitLocation;
		if (GetMouseHitLocation(HitLocation))
		{
			float TerrainHeight;
			if (GetTerrainHeightAtLocation(HitLocation, TerrainHeight)) {
				// Move the preview to the mouse hit location
				FVector TargetLocation = FVector(HitLocation.X, HitLocation.Y, TerrainHeight);

				// Smoothly move the preview towards the target location
				FVector CurrentLocation = SelectedBuilding->GetActorLocation();
				FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), 10.0f); // Adjust interpolation speed

				SelectedBuilding->SetActorLocation(NewLocation);
			}
		}
	}
}

// Get mouse hit location on terrain for building placement
bool ARTSPlayerController::GetMouseHitLocation(FVector& OutHitLocation)
{
	FHitResult HitResult;
	FVector WorldLocation, WorldDirection;

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f);

		if (PerformRaycast(HitResult, Start, End, ECC_Visibility))
		{
			OutHitLocation = HitResult.Location;
			return true;
		}
	}

	return false;
}

// Perform a raycast for terrain and obstacle detection
bool ARTSPlayerController::PerformRaycast(FHitResult& OutHitResult, const FVector& StartLocation, const FVector& EndLocation, ECollisionChannel CollisionChannel)
{
	FCollisionQueryParams QueryParams;
	return GetWorld()->LineTraceSingleByChannel(OutHitResult, StartLocation, EndLocation, CollisionChannel, QueryParams);
}

// Update the minimap with the player's position and other relevant info
void ARTSPlayerController::UpdateMinimap()
{
	if (UMiniMapWidget* MiniMapWidget = RTShud->GetMiniMapWidget())
	{
		FVector PlayerWorldPosition = GetPawn()->GetActorLocation();
		FVector2D MiniMapPosition = MiniMapWidget->WorldToMiniMapPosition(PlayerWorldPosition);

		MiniMapWidget->UpdatePlayerIconPosition(MiniMapPosition);
	}
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

bool ARTSPlayerController::IsLocationFreeOfObstacles(FVector BuildingLocation, FVector BuildingExtents)
{
	// Define the box extents for the building
	FCollisionShape BoxCollision = FCollisionShape::MakeBox(BuildingExtents);

	// Perform a box sweep to detect overlapping actors
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(SelectedBuilding); // Ignore the preview itself

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

bool ARTSPlayerController::GetTerrainHeightAtLocation(const FVector& InLocation, float& OutTerrainHeight)
{
	FHitResult HitResult;
	FVector StartLocation = FVector(InLocation.X, InLocation.Y, InLocation.Z + 1000.0f); // Start above the terrain
	FVector EndLocation = FVector(InLocation.X, InLocation.Y, InLocation.Z - 10000.0f);  // End below the terrain

	if (PerformRaycast(HitResult, StartLocation, EndLocation, ECC_Visibility))
	{
		OutTerrainHeight = HitResult.Location.Z;
		return true;
	}

	return false;
}
