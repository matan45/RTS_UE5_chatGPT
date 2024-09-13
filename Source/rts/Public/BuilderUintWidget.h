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
	// Sets default values for this character's properties
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* buildingIcon;

	UFUNCTION()
	void SeletedFristBuilding();

private:
	class ABuilding* Building;
	class ARTSPlayerController* PlayerController;

};
