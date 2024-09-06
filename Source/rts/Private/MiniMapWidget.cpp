// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"

void UMiniMapWidget::UpdatePlayerIconPosition(FVector2D MiniMapPosition)
{
	if (PlayerIcon)
	{

		UE_LOG(LogTemp, Warning, TEXT("FVector2D: X=%f, Y=%f"), MiniMapPosition.X, MiniMapPosition.Y);
		// Set the PlayerIcon position in the mini-map
		PlayerIcon->SetRenderTranslation(MiniMapPosition);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to link PlayerIcon"));
	}
}
