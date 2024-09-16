// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

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

	void SetPreviewBuildingMesh(bool Visible);
	void SetBuildingMesh(bool Visible);
	void SetStartBuildingMesh(bool Visible);

	

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

	UPROPERTY(EditDefaultsOnly, Category = "Location")
	UMaterialInterface* ValidLocationMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Location")
	UMaterialInterface* InvalidLocationMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Building")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleAnywhere, Category = "Building")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Building")
	UStaticMeshComponent* PreviewBuildingMesh;

	UPROPERTY(VisibleAnywhere, Category = "Building")
	UStaticMeshComponent* StartBuildingMesh;

};
