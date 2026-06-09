// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "D4_HD3_CustomProjectGameMode.generated.h"

class UDeathUI;
/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AD4_HD3_CustomProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AD4_HD3_CustomProjectGameMode();
	
public:
	void RespawnPlayer(ACharacter* Player);
	
};



