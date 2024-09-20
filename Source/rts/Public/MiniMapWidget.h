// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// The image widget that represents the player's position on the minimap
	UPROPERTY(meta = (BindWidget))
	UImage* PlayerIcon;

	// Function to update the PlayerIcon's position on the minimap
	void UpdatePlayerIconPosition(FVector2D MiniMapPosition);

	UPROPERTY(EditAnywhere)
	// Minimap size in pixels (e.g., 512x512)
	FVector2D MiniMapSize;

	UPROPERTY(EditAnywhere)
	// World bounds (e.g., 10000x10000 Unreal units in the game world)
	FVector2D WorldBounds;
	

protected:

	// Convert a world position to minimap coordinates
	FVector2D WorldToMiniMapPosition(FVector WorldPosition) const;

	// Convert a local minimap position (from a mouse click) to world coordinates
	FVector2D MiniMapToWorldPosition(FVector2D LocalPosition) const;

	// Override the OnMouseButtonDown function to handle clicking on the minimap
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
