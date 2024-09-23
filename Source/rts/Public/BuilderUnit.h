// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "BuilderUnit.generated.h"

class UMiniMapWidget;
UCLASS()
class RTS_API ABuilderUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABuilderUnit();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DisplayUI(bool bShowUI);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Builder|Stats")
	float Health = 100.0f;

private:
	
	UPROPERTY(EditAnywhere, Category = "Building|UI")
	TSubclassOf<class UBuilderUintWidget> BuildingSelectionWidgetClass;
	

	// Adding range view for Fog of War
	UPROPERTY(VisibleAnywhere, Category = "Building|FogOfWar")
	USphereComponent* FogOfWarRange;
	UPROPERTY(VisibleAnywhere, Category = "Building|FogOfWar")
	float ViewRage = 1000.0f;
	//TODO 
	//add animation for moving and building
	//add navigating for move location
	//add moving speed

	UBuilderUintWidget* BuildingWidget;
	//add mini map icon
	UMiniMapWidget* MiniMap;
};
