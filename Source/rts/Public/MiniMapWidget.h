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
	// Bind the PlayerIcon from the Blueprint
	UPROPERTY(meta = (BindWidget))
	UImage* PlayerIcon;

	// Function to update the PlayerIcon's position
	UFUNCTION()
	void UpdatePlayerIconPosition(FVector2D MiniMapPosition);
	
};
