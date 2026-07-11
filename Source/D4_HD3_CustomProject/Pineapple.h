// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "Food.h"
#include "Pineapple.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API APineapple : public AFood
{
	GENERATED_BODY()
	
public:
	APineapple();
	
	virtual void EatenBy_Implementation(ACharacter* Character) override;
	
protected:
	UPROPERTY()
	AD4_HD3_CustomProjectCharacter* Player;
	
	FTimerHandle StarvationTimer;
	float EffectDuration = 20.0f;
	void ContinueStarvationDecrement();
	
};
