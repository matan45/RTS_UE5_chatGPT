// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BuilderUnit.generated.h"

UCLASS()
class RTS_API ABuilderUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABuilderUnit();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//TODO 
	//add building widget
	//add mesh
	//add animation for moving and building
	//add hp bar
	//add navigating for move location
	//add moving speed
	//add mini map icon

	UPROPERTY(BlueprintReadWrite, Category = "Building")
	float BuildProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float BuildTime;

	bool bIsBuilding;


};
