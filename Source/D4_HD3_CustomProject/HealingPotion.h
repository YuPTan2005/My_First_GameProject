// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Food.h"
#include "HealingPotion.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API AHealingPotion : public AFood
{
	GENERATED_BODY()
	
public:
	AHealingPotion();
	
	virtual void EatenBy_Implementation(AActor* Character) override;
	
protected:
	float HealingPercent;
	float MaxHealAmount;
	
};
