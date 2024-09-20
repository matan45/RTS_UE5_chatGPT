#include "MiniMapWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Convert the player's world position to minimap coordinates
FVector2D UMiniMapWidget::WorldToMiniMapPosition(FVector WorldPosition) const
{
	// Normalize the world position relative to the world bounds
	FVector2D NormalizedPosition;
	NormalizedPosition.X = (WorldPosition.X + (WorldBounds.X * 0.5f)) / WorldBounds.X;
	NormalizedPosition.Y = (WorldPosition.Y + (WorldBounds.Y * 0.5f)) / WorldBounds.Y;

	// Scale the normalized position to the minimap size
	FVector2D MiniMapPosition = NormalizedPosition * MiniMapSize;

	return MiniMapPosition;
}

// Convert a minimap position (from a mouse click) to a world position
FVector2D UMiniMapWidget::MiniMapToWorldPosition(FVector2D LocalPosition) const
{
	// Normalize the local position within the minimap's size
	FVector2D NormalizedPosition = LocalPosition / MiniMapSize;

	// Scale to the world bounds
	FVector2D WorldPosition;
	WorldPosition.X = NormalizedPosition.X * WorldBounds.X;
	WorldPosition.Y = NormalizedPosition.Y * WorldBounds.Y;

	return WorldPosition;
}

// Update the PlayerIcon's position on the minimap
void UMiniMapWidget::UpdatePlayerIconPosition(FVector2D MiniMapPosition)
{
	if (PlayerIcon)
	{
		// Set the PlayerIcon's position within the minimap using render translation
		PlayerIcon->SetRenderTranslation(MiniMapPosition);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerIcon is null in UpdatePlayerIconPosition"));
	}
}

// Handle mouse clicks on the minimap
FReply UMiniMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Convert the mouse click position on the minimap to local space
	FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	// Convert the minimap position to the corresponding world position
	FVector2D WorldPosition2D = MiniMapToWorldPosition(LocalMousePosition);
	FVector WorldPosition = FVector(WorldPosition2D.X, WorldPosition2D.Y, 0.0f);

	// Optionally perform a ray trace to determine the Z-height of the terrain
	FHitResult HitResult;
	FVector Start = FVector(WorldPosition.X, WorldPosition.Y, 10000.0f);  // Start high above the world
	FVector End = FVector(WorldPosition.X, WorldPosition.Y, -10000.0f);   // Trace downward

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		// Set the Z-height to the terrain height
		WorldPosition.Z = HitResult.Location.Z;
	}

	// Move the player's camera to the clicked world position
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (PlayerController->GetPawn())
		{
			PlayerController->GetPawn()->SetActorLocation(WorldPosition);
		}
	}

	return FReply::Handled();  // Indicate that the click event was handled
}