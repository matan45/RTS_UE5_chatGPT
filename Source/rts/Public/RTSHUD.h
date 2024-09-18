// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.h"
#include "RTSHUD.generated.h"

/**
 *
 */
UCLASS()
class RTS_API ARTSHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Accessor for MiniMap widget
	UMiniMapWidget* GetMiniMapWidget() const { return MiniMapWidget; }

protected:
	// Reference to the UI widget class for displaying game time
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameTimeWidgetClass;

	// Reference to the UI widget class for the mini-map
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MiniMapWidgetClass;

	// Reference to the widget instance of the game time display
	UPROPERTY()
	UUserWidget* GameTimeWidget;

	// Reference to the MiniMap widget instance
	UPROPERTY()
	UMiniMapWidget* MiniMapWidget;

private:
	// Initializes the Game Time widget and connects it to the GameTimeManager
	void SetUpTimeWidget();

	// Initializes the MiniMap widget
	void SetUpMiniMapWidget();

	// Helper function to update the time display based on the GameTimeManager
	UFUNCTION()
	void UpdateTimeDisplay(int32 Hours, int32 Minutes, int32 Seconds);

	// Cached reference to the GameTimeManager
	class AGameTimeManager* GameTimeManager;

};
