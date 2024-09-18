#include "RTSHUD.h"
#include "GameTimeManager.h"
#include "Kismet/GameplayStatics.h"


void ARTSHUD::BeginPlay()
{
	Super::BeginPlay();
	// Set up the Game Time widget
	SetUpTimeWidget();

	// Set up the MiniMap widget
	SetUpMiniMapWidget();
}

void ARTSHUD::SetUpTimeWidget()
{
	if (GameTimeWidgetClass)
	{
		// Create the widget from the class
		GameTimeWidget = CreateWidget<UUserWidget>(GetWorld(), GameTimeWidgetClass);

		if (GameTimeWidget)
		{
			GameTimeWidget->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("GameTimeWidget created and added to viewport"));

			// Find the GameTimeManager in the world
			GameTimeManager = Cast<AGameTimeManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameTimeManager::StaticClass()));

			if (GameTimeManager)
			{
				UE_LOG(LogTemp, Log, TEXT("GameTimeManager found"));

				// Bind the hour update event to a function that updates the HUD
				GameTimeManager->OnTimeUpdated.AddDynamic(this, &ARTSHUD::UpdateTimeDisplay);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("GameTimeManager not found in world"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create GameTimeWidget"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameTimeWidgetClass is null"));
	}
}

// Initializes the MiniMap widget
void ARTSHUD::SetUpMiniMapWidget()
{
	if (MiniMapWidgetClass)
	{
		// Create the MiniMap widget from the class
		MiniMapWidget = CreateWidget<UMiniMapWidget>(GetWorld(), MiniMapWidgetClass);

		if (MiniMapWidget)
		{
			MiniMapWidget->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("MiniMapWidget created and added to viewport"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create MiniMapWidget"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MiniMapWidgetClass is null"));
	}
}

// Function to update the time display when an hour passes (using Blueprint logic or a custom widget)
void ARTSHUD::UpdateTimeDisplay(int32 Hours, int32 Minutes, int32 Seconds)
{
	// Assuming your widget has a method to update the time display, use it here
	if (GameTimeWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("GameTimeWidget found"));

		UFunction* UpdateTimeFunc = GameTimeWidget->FindFunction(FName("UpdateTimeDisplay"));

		if (UpdateTimeFunc)
		{
			UE_LOG(LogTemp, Log, TEXT("UpdateTimeFunc found"));
			struct FTimeUpdateParams
			{
				int32 InHoursPassed;
				int32 InMinutesPassed;
				int32 InSecondsPassed;
			};

			FTimeUpdateParams Params;
			Params.InHoursPassed = Hours;
			Params.InMinutesPassed = Minutes;
			Params.InSecondsPassed = Seconds;

			GameTimeWidget->ProcessEvent(UpdateTimeFunc, &Params);
			UE_LOG(LogTemp, Log, TEXT("Updated time display in GameTimeWidget to %02d:%02d:%02d"), Hours, Minutes, Seconds);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateTimeDisplay function not found in GameTimeWidget"));
		}

	}
}

