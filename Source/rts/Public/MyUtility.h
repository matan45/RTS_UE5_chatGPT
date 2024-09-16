// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyUtility.generated.h"
/**
 * 
 */
UCLASS()
class RTS_API UMyUtility : public UObject
{
	GENERATED_BODY()
public:
	// Define static utility functions

	// Min4: Find the minimum of four float values
	static float Min4(float A, float B, float C, float D);
	// Max4: Find the maximum of four float values
	static float Max4(float A, float B, float C, float D);
};
