// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "BuilderUintWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_API UBuilderUintWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* buildingIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	TSubclassOf<class ABuilding> BuildingClass; // Allows for Blueprint-specified building types

	UFUNCTION()
	void SeletedFristBuilding();

private:
	class ABuilding* Building;
	class ARTSPlayerController* PlayerController;

};
