// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "D4_HD3_CustomProjectGameMode.generated.h"

class UDeathUI;
/**
 *  Simple GameMode for a third person game
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRespawned);

UCLASS(abstract)
class AD4_HD3_CustomProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AD4_HD3_CustomProjectGameMode();

	UPROPERTY(BlueprintAssignable, Category = "Game Events")
	FOnPlayerRespawned OnPlayerRespawned;
	
	void RespawnPlayer(const ACharacter* Player);
	
};



