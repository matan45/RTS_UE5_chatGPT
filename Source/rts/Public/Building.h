// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"


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
	bool IsPreviewBuildingMesh() const;
	bool IsBuildingMesh() const;
	bool IsStartBuildingMesh() const;
	FVector GetBuildingExtents() {
		return PreviewBuildingMesh->GetStaticMesh()->GetBounds().BoxExtent;
	}

	// Function to change the building state and update mesh visibility accordingly
	void SetBuildingState(EBuildingState NewState);
	// Helper to update materials based on validity of placement
	void UpdatePlacementMaterial(bool bIsValid);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//TODO 
	//add unit widget ui for selet with building to build
	//add mesh
	//add hp
	//add queue for training units
	//add really point
	//add mini map icon
	//show constraction proggres bar
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

};
