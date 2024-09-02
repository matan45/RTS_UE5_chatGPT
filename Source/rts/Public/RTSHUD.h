// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
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

protected:
	// Reference to the UI widget class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameTimeWidgetClass;

	// Reference to the widget instance
	UPROPERTY()
	UUserWidget* GameTimeWidget;
	
};
