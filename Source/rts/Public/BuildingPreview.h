// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingPreview.generated.h"

UCLASS()
class RTS_API ABuildingPreview : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingPreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FinishPlacingBuilding();
	void SetIsValidLocation(bool bIsValid);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Location")
	class UMaterialInterface* ValidLocationMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Location")
	class UMaterialInterface* InvalidLocationMaterial;

};
