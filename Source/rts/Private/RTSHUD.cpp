#include "RTSHUD.h"
#include "GameTimeManager.h"
#include "Kismet/GameplayStatics.h"


void ARTSHUD::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Start"));

	if (GameTimeWidgetClass)
	{
		// Create the widget using the class member variable
		GameTimeWidget = CreateWidget<UUserWidget>(GetWorld(), GameTimeWidgetClass);
		if (GameTimeWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameTimeWidget created successfully"));

			// Find the GameTimeManager instance
			AGameTimeManager* GameTimeManager = Cast<AGameTimeManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameTimeManager::StaticClass()));

			if (GameTimeManager)
			{
				UE_LOG(LogTemp, Warning, TEXT("GameTimeManager found"));

				// Assuming your widget has a method to receive the GameTimeManager reference
				UFunction* SetGameTimeManagerFunc = GameTimeWidget->FindFunction(FName("SetGameTimeManager"));
				if (SetGameTimeManagerFunc)
				{
					struct FGameTimeManagerParams
					{
						AGameTimeManager* Manager;
					};

					UE_LOG(LogTemp, Warning, TEXT("SetGameTimeManagerFunc found"));

					FGameTimeManagerParams Params;
					Params.Manager = GameTimeManager;
					GameTimeWidget->ProcessEvent(SetGameTimeManagerFunc, &Params);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("GameTimeManager not found"));
			}

			// Add the widget to the viewport
			GameTimeWidget->AddToViewport();
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

	UUserWidget* MiniMapWidget = CreateWidget<UUserWidget>(GetWorld(), MiniMapWidgetClass);
	if (MiniMapWidget)
	{
		MiniMapWidget->AddToViewport();
	}
}

