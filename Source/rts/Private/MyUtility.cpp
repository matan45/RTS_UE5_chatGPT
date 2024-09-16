// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUtility.h"


float UMyUtility::Min4(float A, float B, float C, float D)
{
	return FMath::Min(FMath::Min(A, B), FMath::Min(C, D));
}

float UMyUtility::Max4(float A, float B, float C, float D)
{
	return FMath::Max(FMath::Max(A, B), FMath::Max(C, D));
}
