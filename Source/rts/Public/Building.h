// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Building.generated.h"

class UMiniMapWidget;

UENUM(BlueprintType)
enum class EBuildingState : uint8
{
	Preview UMETA(DisplayName = "Preview"),
	UnderConstruction UMETA(DisplayName = "Under Construction"),
	Completed UMETA(DisplayName = "Completed")
};

UCLASS()
class RTS_API ABuilding : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuilding();

	virtual void Tick(float DeltaTime) override;

	FVector GetBuildingExtents() {
		return PreviewBuildingMesh->GetStaticMesh()->GetBounds().BoxExtent;
	}

	EBuildingState GetBuildingState() {
		return CurrentState;
	}

	// Function to change the building state and update mesh visibility accordingly
	void SetBuildingState(EBuildingState NewState);
	// Helper to update materials based on validity of placement
	void UpdatePlacementMaterial(bool bIsValid);

	virtual float TakeDamage(
		float DamageAmount,
		const FDamageEvent& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;
	void Construction(float DeltaTime);

	// Function to add a unit to the training queue
	void AddUnitToQueue(TSubclassOf<AActor> UnitClass);

	// Function to start training the next unit in the queue
	void TrainNextUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Adding range view for Fog of War
	UPROPERTY(VisibleAnywhere, Category = "Building|FogOfWar")
	USphereComponent* FogOfWarRange;
	UPROPERTY(VisibleAnywhere, Category = "Building|FogOfWar")
	float ViewRage = 1000.0f;

	//Hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Stats")
	float BuildingHealth = 100.0f;

	//really point
	UPROPERTY(BlueprintReadWrite, Category = "Building|Rally")
	FVector RallyPoint;

	UPROPERTY(BlueprintReadWrite, Category = "Building|Construction")
	// Placeholder for construction time and progress
	float ConstructionTime = 10.0f;
	float ElapsedConstructionTime = 0.0f;
	
	// Unit training queue and timer
	UPROPERTY(VisibleAnywhere, Category = "Building|Training")
	TArray<TSubclassOf<AActor>> TrainingQueue;

	UPROPERTY(EditAnywhere, Category = "Building|Training")
	float UnitTrainingTime;  // Time to train each unit

	FTimerHandle TrainingTimerHandle;

	
private:

	// Dynamic materials to show valid/invalid placement
	UPROPERTY(EditDefaultsOnly, Category = "Building|Materials")
	UMaterialInterface* ValidLocationMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Building|Materials")
	UMaterialInterface* InvalidLocationMaterial;

	// Building meshes
	UPROPERTY(VisibleAnywhere, Category = "Building|Mesh")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleAnywhere, Category = "Building|Mesh")
	UStaticMeshComponent* PreviewBuildingMesh;

	UPROPERTY(VisibleAnywhere, Category = "Building|Mesh")
	UStaticMeshComponent* StartBuildingMesh;

	// To store the building state (Preview, Under Construction, Completed)
	EBuildingState CurrentState;

	void SetPreviewBuildingMesh(bool Visible);
	void SetBuildingMesh(bool Visible);
	void SetStartBuildingMesh(bool Visible);

	// Function to handle spawning a unit after training
	void SpawnTrainedUnit(TSubclassOf<AActor> UnitClass);

	//add mini map icon
	UMiniMapWidget* MiniMap;
};
