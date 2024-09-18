// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"

#include <Kismet/GameplayStatics.h>

void UMiniMapWidget::UpdatePlayerIconPosition(FVector2D MiniMapPosition)
{
	if (PlayerIcon)
	{
		// Set the PlayerIcon position in the mini-map
		PlayerIcon->SetRenderTranslation(MiniMapPosition);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to link PlayerIcon"));
	}
}

FReply UMiniMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Get the local mouse position relative to the widget
	FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	// Define the world size that the mini-map represents
	FVector2D WorldSize = FVector2D(10000.0f, 10000.0f); // Adjust this based on your world

	// Get the size of the mini-map widget (e.g., 512x512 pixels)
	FVector2D MiniMapSize = InGeometry.GetLocalSize();

	// Normalize the local mouse position within the mini-map (0.0 to 1.0)
	FVector2D NormalizedPosition = LocalMousePosition / MiniMapSize;

	// Convert the normalized 2D position to a 3D world position
	FVector WorldPosition;
	WorldPosition.X = NormalizedPosition.X * WorldSize.X;
	WorldPosition.Y = NormalizedPosition.Y * WorldSize.Y;
	WorldPosition.Z = 0.0f;  // Set Z based on your game (e.g., terrain height, or keep it 0 for flat ground)

	// Perform a line trace to find the Z-height of the terrain
	FHitResult HitResult;
	FVector Start = FVector(WorldPosition.X, WorldPosition.Y, 10000.0f);  // Start high above the world
	FVector End = FVector(WorldPosition.X, WorldPosition.Y, -10000.0f);   // Trace downwards

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		// Set the Z to the hit location (e.g., ground)
		WorldPosition.Z = HitResult.Location.Z + 1000;
	}

	// Move the player's pawn or camera to the target location
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			// Smoothly move the player's pawn to the target position over time
			FVector CurrentLocation = PlayerPawn->GetActorLocation();
			FVector TargetLocation = WorldPosition;

			// Speed of the camera movement
			float MoveSpeed = 50.0f;  // Adjust speed as necessary

			// Interpolating between the current location and the target location
			FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), MoveSpeed);

			// Update pawn's location
			PlayerPawn->SetActorLocation(NewLocation);
		}
	}
	return FReply::Handled();  // This indicates that the click event was handled.

}
