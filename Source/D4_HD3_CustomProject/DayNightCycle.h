// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "Engine/LevelScriptActor.h"
#include "DayNightCycle.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API ADayNightCycle : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ADayNightCycle();
	
protected:
	float DaySpeed = 1.0f;
	
	UPROPERTY()
	ADirectionalLight* SunLight;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
};
