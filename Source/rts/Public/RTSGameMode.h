// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"


class AGameTimeManager;

#include "RTSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RTS_API ARTSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARTSGameMode();

	virtual void BeginPlay() override;

	AGameTimeManager* GetGameTimeManager() const { return GameTimeManager; }


private:
	AGameTimeManager* GameTimeManager;
	
};
